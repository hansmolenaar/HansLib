#include "MeshGeneration2.h"
#include "BoundingBox.h"
#include "Defines.h"
#include "Functors.h"
#include "IntervalTreeBalance.h"
#include "Manifold0Reconstruction.h"
#include "Manifold1Reconstruction.h"
#include "MeshGenerationUtils.h"
#include "MyAssert.h"
#include "Polygon2D.h"
#include "StdHash.h"
#include "Triangle.h"
#include "UniquePointCollectionBinning.h"

#include <set>

using namespace Geometry;
using namespace IntervalTree;
using namespace MeshGeneration;
using namespace MeshGeneration2;
using namespace Topology;

namespace
{
using M0ToTriIds = std::multimap<const IManifold0D2 *, CellIndex>;
M0ToTriIds GetTrianglesContainingPoints(std::span<const IManifold0D2 *> manifolds,
                                        MeshGeneration::TrianglesNodes &trianglesNodes,
                                        IUniquePointCollection2 &pointCollection)
{
    M0ToTriIds result;
    constexpr double eps = 0.01;
    const auto scale = BoundingBox<GeomType, GeomDim2>::CreateFromList(
        std::vector<Point2>{Point2{-eps, -eps}, Point2{1 + eps, 1 + eps}});
    for (auto triangleId : trianglesNodes.getAllTriangles())
    {
        const auto triangle =
            GetTriangleGeometry(trianglesNodes.getTriangleNodes(triangleId).asTriangleNodes(), pointCollection);
        const auto bb = BoundingBox<GeomType, GeomDim2>::CreateFromList(triangle).scaleFrom01(scale);
        for (auto mptr : manifolds)
        {
            const auto point = mptr->getPoint();
            if (bb.contains(point))
            {
                if (Polygon2D::Contains((std::span<const Point<GeomType, 2>>)triangle, point,
                                        pointCollection.getGeometryPredicate()))
                {
                    result.emplace(mptr, triangleId);
                }
            }
        }
    }
    return result;
}

// Explicit sorting, otherwise depence on pointer ordering
std::vector<const IManifold0D2 *> OrderManifolds0(std::span<const IManifold0D2 *> manifolds)
{
    std::vector<const IManifold0D2 *> result(manifolds.begin(), manifolds.end());
    auto lessPoint = [](const IManifold0D2 *ptr1, const IManifold0D2 *ptr2) {
        return ptr1->getPoint() < ptr2->getPoint();
    };
    str::sort(result, lessPoint);
    return result;
}

NodeIndex InsertPoint(Point2 point, std::span<const CellIndex> candidates, TrianglesNodes &trianglesNodes,
                      IDynamicUniquePointCollection2 &pointCollection,
                      const MeshGeneration::ManifoldsAndNodes<GeomDim2> &manifoldsAndNodes)
{
    // Find the nearest triangle node
    double minDist2 = std::numeric_limits<double>::max();
    NodeIndex minDist2Node = NodeIndexInvalid;
    for (auto candidateTriangle : candidates)
    {
        const auto triangle = trianglesNodes.getTriangleNodes(candidateTriangle);
        for (auto n : triangle)
        {
            const Point<double, GeomDim2> trianglePoint = pointCollection.getPoint(n);
            const double dist2 = PointUtils::GetDistanceSquared(point, trianglePoint);
            if (dist2 < minDist2)
            {
                minDist2 = dist2;
                minDist2Node = n;
            }
        }
    }

    for (const auto *manifold : manifoldsAndNodes.getManifoldsContainingNode(minDist2Node))
    {
        if (!manifoldsAndNodes.isMobileOnManifold(minDist2Node, manifold))
        {
            throw MyException("MeshGeneration2::InsertPoint immovable node");
        }
    }

    pointCollection.movePoint(minDist2Node, point);
    return minDist2Node;
}
} // namespace

IndexTreeToSimplices2::Triangles MeshGeneration2::GenerateBaseTriangulation(
    const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2> &region, IMeshingSettings2 &settings)
{
    auto &logger = settings.getLogger();
    std::ostringstream os;
    os << "MeshGeneration2::GenerateBaseTriangulation() region Bb " << region.getBoundingBox();
    logger.logLine(os);

    IndexTree<GeomDim2> tree;
    auto refinementPredicate = settings.getRefinementPredicateFactory().Create(region, settings.getInitialBbGenerator(),
                                                                               settings.getGeometryPredicate());
    tree.refineUntilReady(*refinementPredicate);
    os.str("");
    os << "MeshGeneration2::GenerateBaseTriangulation() after refinement: " << tree;
    logger.logLine(os);

    IntervalTree::Balance(tree);
    os.str("");
    os << "MeshGeneration2::GenerateBaseTriangulation() after balancing (max 1 level difference): " << tree;
    logger.logLine(os);

    const auto result = IndexTreeToSimplices2::Create(tree);
    os.str("");
    os << "MeshGeneration2::GenerateBaseTriangulation(): number of triangles " << result.size();
    logger.logLine(os);

    return result;
}

void MeshGeneration2::BaseTriangulationToWorld(const IndexTreeToSimplices2::Triangles &baseTriangles,
                                               const IGeometryPredicate<GeomType, GeomDim2> &predicate,
                                               const BoundingBox<GeomType, GeomDim2> &worldBB,
                                               std::unique_ptr<IDynamicUniquePointCollection2> &pointGeometry,
                                               TrianglesNodes &triangleNodes, Logger &logger)
{
    std::ostringstream os;

    // Clear output
    pointGeometry.reset();
    if (triangleNodes.getNumTriangles() != 0)
    {
        throw MyException("BaseTriangulationToWorld triangleNodes should be empty");
    }

    // Collect unique points
    std::unordered_map<RatPoint2, Point2, ArrayHasher<Rational, 2>> uniquePoints;
    std::vector<Point2> uniqueWorldPoints;
    for (auto &triangle : baseTriangles)
    {
        for (auto &p : triangle)
        {
            if (!uniquePoints.contains(p))
            {
                const auto worldPoint = worldBB.scaleFromPoint01(p);
                uniquePoints[p] = worldPoint;
                uniqueWorldPoints.push_back(worldPoint);
            }
        }
    }

    logger.logLine("MeshGeneration2::BaseTriangulationToWorld number of unique points: " +
                   std::to_string(uniquePoints.size()));

    // Build the point collection from the points

    auto binnedCollection = std::make_unique<UniquePointCollectionBinning<2>>(predicate, uniqueWorldPoints);
    os.str("");
    os << "MeshGeneration2::BaseTriangulationToWorld binning collecion: " << *binnedCollection;
    logger.logLine(os);

    pointGeometry.reset(binnedCollection.release());

    std::unordered_map<RatPoint2, PointIndex, ArrayHasher<Rational, 2>> toWorld;

    // Get IDs for the points
    for (auto &up : uniquePoints)
    {
        const PointIndex worldId = *pointGeometry->tryGetClosePoint(up.second);
        toWorld.emplace(up.first, worldId);
    }

    // Create the triangles
    for (auto &triangle : baseTriangles)
    {
        const auto n0 = toWorld.at(triangle.at(0));
        const auto n1 = toWorld.at(triangle.at(1));
        const auto n2 = toWorld.at(triangle.at(2));
        triangleNodes.addTriangle(TriangleNodesOriented(n0, n1, n2));
    }

    os.str("");
    os << "MeshGeneration2::BaseTriangulationToWorld topology\n" << triangleNodes;
    logger.logLine(os);
}

static boost::container::static_vector<NodeIndex, 2> HandleEndPoints(
    DirectedEdgeIntersections<GeomType, GeomDim2> &intersections, const Geometry::IManifold1D2<GeomType> &manifold,
    const EdgeNodesDirected &edgeNodes, ManifoldsAndNodes<GeomDim2> &manifoldsAndNodes)
{
    boost::container::static_vector<NodeIndex, 2> nodeUsed;
    if (intersections.get().empty())
        return nodeUsed;

    // Loop backwards so that we can erase both end-points
    for (int n = static_cast<int>(intersections.get().size()) - 1; n >= 0; --n)
    {
        if (intersections[n].isIsolatedPoint())
        {
            const auto &ip = intersections[n].getIsolatedPoint();
            if (ip.getPointType() != DirectedEdgePointType::Inside)
            {
                const auto node = (ip.getPointType() == DirectedEdgePointType::Point0 ? edgeNodes[0] : edgeNodes[1]);
                nodeUsed.push_back(node);
                manifoldsAndNodes.addNodeToManifold(node, &manifold);
                intersections.erase(n);
            }
        }
        else
        {
            // Only implemented: intersected on both sides
            const auto &interval = intersections[n].getInterval();
            if (interval.Point0.getPointType() == DirectedEdgePointType::Inside ||
                interval.Point1.getPointType() == DirectedEdgePointType::Inside)
            {
                throw MyException("HandleEndPoints expect both end-points");
            }
            const auto minNode = std::min(edgeNodes[0], edgeNodes[1]);
            const auto maxNode = std::max(edgeNodes[0], edgeNodes[1]);
            nodeUsed.push_back(minNode);
            nodeUsed.push_back(maxNode);
            manifoldsAndNodes.addNodeToManifold(minNode, &manifold);
            manifoldsAndNodes.addNodeToManifold(maxNode, &manifold);
            intersections.erase(n);
        }
    }
    return nodeUsed;
}

// Return value: any node moved?
bool MeshGeneration2::AddEdgeManifold1Intersections(const Geometry::IManifold1D2<GeomType> &manifold,
                                                    const EdgeNodesDirected &edgeNodes,
                                                    const TrianglesNodes &trianglesNodes,
                                                    ManifoldsAndNodes<GeomDim2> &manifoldsAndNodes,
                                                    IDynamicUniquePointCollection2 &pointCollection)
{
    const auto &predicate = pointCollection.getGeometryPredicate();
    const DirectedEdge<GeomType, GeomDim2> edge(pointCollection.getPoint(edgeNodes[0]),
                                                pointCollection.getPoint(edgeNodes[1]));
    auto intersections = manifold.getIntersections(edge, predicate);
    if (intersections.get().empty())
        return false; // Nothing to do

    const auto nodeUsed = HandleEndPoints(intersections, manifold, edgeNodes, manifoldsAndNodes);
    if (intersections.get().empty())
        return false;

    if (intersections.get().size() == 1)
    {
        if (!intersections[0].isIsolatedPoint())
        {
            throw MyException("Intersection interval not yet implemented");
        }
        const auto &ip = intersections[0].getIsolatedPoint();
        const auto dist0 = PointUtils::GetDistanceSquared(ip.getPoint(), edge.point0());
        const auto dist1 = PointUtils::GetDistanceSquared(ip.getPoint(), edge.point1());
        auto nodeToMove = dist0 < dist1 ? edgeNodes[0] : edgeNodes[1];
        if (str::find(nodeUsed, nodeToMove) != nodeUsed.end() ||
            !manifoldsAndNodes.isMobileOnManifold(nodeToMove, &manifold))
        {
            // Take the other one
            nodeToMove = (nodeToMove == edgeNodes[0]) ? edgeNodes[1] : edgeNodes[0];
        }

        if (!manifoldsAndNodes.isMobileOnManifold(nodeToMove, &manifold))
        {
            throw MyException("MeshGeneration2::AddEdgeManifold1Intersections immovable node");
        }
        pointCollection.movePoint(nodeToMove, ip.getPoint());
        manifoldsAndNodes.addNodeToManifold(nodeToMove, &manifold);
    }
    else
    {
        Utilities::MyAssert(intersections.get().size() == 2);
        for (int n = 0; n < 2; ++n)
        {
            if (!intersections[n].isIsolatedPoint())
            {
                throw MyException("Intersection interval not yet implemented, intersection = " + std::to_string(n));
            }
            const auto &ip = intersections[n].getIsolatedPoint();
            // Automatically ordered: directed edge
            const auto nodeToMove = edgeNodes[n];
            if (!manifoldsAndNodes.isMobileOnManifold(nodeToMove, &manifold))
            {
                throw MyException("MeshGeneration2::AddEdgeManifold1Intersections immovable node");
            }
            pointCollection.movePoint(nodeToMove, ip.getPoint());
            manifoldsAndNodes.addNodeToManifold(nodeToMove, &manifold);
        }
    }
    return true;
}

void MeshGeneration2::AddManifold1Intersections(const Geometry::IManifold1D2<GeomType> &manifold,
                                                TrianglesNodes &trianglesNodes,
                                                MeshGeneration::ManifoldsAndNodes<GeomDim2> &manifoldsAndNodes,
                                                IDynamicUniquePointCollection2 &pointCollection, Logger &logger)
{
    // LogTriangles(logger, trianglesNodes, "trianglesNodes");
    const auto initialSortedEdges = trianglesNodes.getAllSortedEdges();
    // Log(logger, initialSortedEdges, "initialSortedEdges ");
    std::set<EdgeNodesSorted> todo(initialSortedEdges.begin(), initialSortedEdges.end());
    while (!todo.empty())
    {
        const auto edge = *todo.begin();
        todo.erase(edge);
        const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, {edge[0], edge[1]}, trianglesNodes,
                                                                manifoldsAndNodes, pointCollection);
        if (anyNodeMoved)
        {
            for (NodeIndex node : edge)
            {
                for (NodeIndex ngb : trianglesNodes.getEdgeConnectedNodes(node))
                {
                    const EdgeNodesSorted todoEdge{node, ngb};
                    todo.insert(todoEdge);
                }
            }
        }
    }
}

void MeshGeneration2::AddAllManifolds0(std::span<const IManifold0D2 *> manifolds,
                                       MeshGeneration::TrianglesNodes &trianglesNodes,
                                       MeshGeneration::ManifoldsAndNodes<GeomDim2> &manifoldsAndNodes,
                                       IDynamicUniquePointCollection2 &pointCollection, Logger &logger)
{
    // Collect triangles that contain point
    const auto trianglesContainingPoints = GetTrianglesContainingPoints(manifolds, trianglesNodes, pointCollection);

    for (const auto *manifold : OrderManifolds0(manifolds))
    {
        const auto range = trianglesContainingPoints.equal_range(manifold);
        std::vector<CellIndex> candidates;
        std::transform(range.first, range.second, std::back_inserter(candidates),
                       [](const auto &itr) { return itr.second; });
        const NodeIndex node =
            InsertPoint(manifold->getPoint(), candidates, trianglesNodes, pointCollection, manifoldsAndNodes);
        manifoldsAndNodes.addNodeToManifold(node, manifold);
        std::ostringstream os;
        os << "Added point manifold " << manifold->getName() << " with point " << manifold->getPoint() << "as node "
           << node;
        logger.logLine(os.str());
    }
}

std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>> MeshGeneration2::createReconstructions(
    const IRegionManifolds<GeomType, GeomDim2> &regionManifolds, const TrianglesNodes &trianglesNodes,
    const ManifoldsAndNodes<GeomDim2> &manifoldsAndNodes, const IUniquePointCollection2 &pointCollection)
{
    std::vector<std::unique_ptr<IManifoldReconstruction>> result;
    const auto allManifolds = regionManifolds.getAllManifolds();
    auto cmp = [](const IManifoldId *a, const IManifoldId *b) { return *a < *b; };
    std::set<const IManifoldId *, decltype(cmp)> manifoldIds;
    for (const auto *m : allManifolds)
    {
        if (manifoldIds.contains(m))
        {
            throw MyException("MeshGeneration2::createReconstructions Duplicate manifold " + m->getName());
        }
        manifoldIds.insert(m);

        if (m->getTopologyDimension() == Topology::Corner)
        {
            const auto point = dynamic_cast<const IManifold0<GeomType, GeomDim2> *>(m)->getPoint();
            const auto found = pointCollection.tryGetClosePoint(point);
            if (found)
            {
                result.emplace_back(std::make_unique<Manifold0Reconstruction>(*m, Reconstruction0{*found}));
            }
        }
        else if (m->getTopologyDimension() == Topology::Edge)
        {
            const auto *m2 = dynamic_cast<const Geometry::IManifold1D2<GeomType> *>(m);
            result.emplace_back(std::make_unique<Manifold1Reconstruction>(*m2, manifoldsAndNodes, trianglesNodes));
        }
        else
        {
            throw MyException("MeshGeneration2::createReconstructions unexpect dimension " +
                              std::to_string(m->getTopologyDimension()) + " for manifold " + m->getName());
        }
    }

    // manifold topology
    return result;
}

bool MeshGeneration2::checkReconstructions(
    const Geometry::IRegionManifolds<MeshGeneration::GeomType, GeomDim2> &regionManifolds,
    const std::vector<const MeshGeneration::IManifoldReconstruction *> &reconstructions, Logger &logger)
{
    bool succes = true;
    const auto allManifolds = regionManifolds.getAllManifolds();

    // All point manifolds found?
    std::vector<const IManifoldId *> reconstructedPointManifoldIds;
    for (const auto *m : regionManifolds.getManifoldsOfType<const IManifold0<GeomType, GeomDim2> *>())
    {
        const auto found = str::find_if(reconstructions, [&m](const auto *ptr) { return *m == ptr->getManifoldId(); });
        if (found == reconstructions.end())
        {
            succes = false;
            logger.logLine("MeshGeneration2::createReconstructions point manifold " + m->getName() +
                           " not found in mesh");
        }
        else
        {
            reconstructedPointManifoldIds.push_back(&((*found)->getManifoldId()));
        }
    }

    // Connection between corner and edge manifolds
    for (const auto *m : regionManifolds.getManifoldsOfType<const IManifold1<GeomType, GeomDim2> *>())
    {
        const auto lowers = regionManifolds.getConnectedLowers(*m);
        for (const auto lm : lowers)
        {
            const auto found = str::find_if(reconstructedPointManifoldIds, [&lm](const auto *r) { return *r == *lm; });
            if (found == reconstructedPointManifoldIds.end())
            {
                succes = false;
                logger.logLine("MeshGeneration2::createReconstructions edge " + m->getName() +
                               " connected point manifold " + lm->getName() + " not found in mesh");
            }
        }
    }

    // End-points?
    for (const auto *m : regionManifolds.getManifoldsOfType<const IManifold1<GeomType, GeomDim2> *>())
    {
        const auto endPoints = regionManifolds.getConnectedLowers(*m);
        size_t numEndPointsInReconstruction = 0;
        auto cast1 = [](const auto *r) { return dynamic_cast<const Manifold1Reconstruction *>(r); };
        auto pred = [&m](const auto *r1) { return r1 != nullptr && *m == r1->getManifoldId(); };
        for (const auto &reconstruction : reconstructions | stv::transform(cast1) | stv::filter(pred))
        {
            numEndPointsInReconstruction += 2 * reconstruction->getReconstruction().getPaths().size();
        }

        if (endPoints.size() != numEndPointsInReconstruction)
        {
            succes = false;
            logger.logLine("MeshGeneration2::createReconstructions edge " + m->getName() + " has " +
                           std::to_string(endPoints.size()) + "end-points, in the reconstruction there are " +
                           std::to_string(numEndPointsInReconstruction) + " end-ponts");
        }
    }

    return succes;
}
