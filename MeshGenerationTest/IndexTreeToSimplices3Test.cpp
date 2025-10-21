#include <gtest/gtest.h>

#include "Boundary1.h"
#include "ITopologicalAdjacency.h"
#include "IndexTreeToSimplices3.h"
#include "IntervalTreeBalance.h"
#include "IntervalTreeRefinePredicate.h"
#include "IntervalTreeVtk.h"
#include "Paraview.h"
#include "ProjectToVtk.h"
#include "ReferenceShapeCube.h"
#include "Tetrahedron.h"
#include "TetrahedronsNodes.h"
#include "TopologyDefines.h"
#include "UniqueHashedPointCollection.h"

using namespace IntervalTree;
using namespace IndexTreeToSimplices3;
using namespace Topology;
using namespace MeshGeneration;

namespace
{
void testOrientation(const TetrahedronNodesOriented &tet, const IPointCollection3 &points)
{
    const double volume = Tetrahedron::getSignedVolume(points.getPoint(tet[0]), points.getPoint(tet[1]),
                                                       points.getPoint(tet[2]), points.getPoint(tet[3]));
    ASSERT_GT(volume, 0.0);
}

void checkMesh(const TetrahedronsNodes &tets, const IPointCollection3 &points)
{
    for (auto cellId : tets.getAllTetrahedrons())
    {
        const auto tet = tets.getTetrahedronNodes(cellId);
        testOrientation(tet, points);
    }
}

struct ContainsPoint
{
    bool operator()(const Index<GeomDim3> &indx) const
    {
        const auto &bbNodes = IndexTreeToSimplices3::getCubeFromIndex(indx);
        return str::any_of(
            bbNodes, [this](const RatPoint3 &p) { return p[0] == point[0] && p[1] == point[1] && p[2] == point[2]; });
    }
    RatPoint3 point;
};
} // namespace

TEST(IndexTreeToSimplices3Test, RootToVtk)
{
    IndexTree<GeomDim3> tree;
    const auto &root = tree.getRoot();

    const auto tets = IndexTreeToSimplices3::Create(tree);
    ASSERT_EQ(ReferenceShapeCube::numTetsInStandardSplit, tets.size());
    TetrahedronsNodes allTets;
    UniqueHashedPointCollection<double, GeomDim3> points;
    for (const auto &tet : tets)
    {
        std::array<NodeIndex, Topology::NumCornersOnTetrahedron> nodes;
        for (size_t pos = 0; auto n : tet)
        {
            nodes[pos] = points.addIfNew(PointUtils::toPoint(n));
            ++pos;
        }
        allTets.addTetrahedron(TetrahedronNodesOriented(nodes));
    }
    checkMesh(allTets, points);

    ProjectToVtk toVtk("IndexTreeToSimplices3Test_RootToVtk");
    IndexTreeToSimplices3::cellsToVtkData(toVtk, tets);
    const auto *vtkMesh = toVtk.get().front();
    ASSERT_EQ(ReferenceShapeCube::numTetsInStandardSplit, vtkMesh->getNumCells());
    ASSERT_EQ(NumCornersOnCube, vtkMesh->getNumNodes());
    // Paraview::Paraview::WriteList(vtkData);
}

TEST(IndexTreeToSimplices3Test, Level1ToVtk)
{
    IndexTree<GeomDim3> tree;
    const auto &root = tree.getRoot();
    RefineToMaxLevel<GeomDim3> doRefine1{1};
    tree.refineLeaves(doRefine1);

    const auto tets = IndexTreeToSimplices3::Create(tree);
    ASSERT_EQ(8 * ReferenceShapeCube::numTetsInStandardSplit, tets.size());

    TetrahedronsNodes allTets;
    UniqueHashedPointCollection<double, GeomDim3> points;
    for (const auto &tet : tets)
    {
        std::array<NodeIndex, Topology::NumCornersOnTetrahedron> nodes;
        for (size_t pos = 0; auto n : tet)
        {
            nodes[pos] = points.addIfNew(PointUtils::toPoint(n));
            ++pos;
        }
        allTets.addTetrahedron(TetrahedronNodesOriented(nodes));
    }
    checkMesh(allTets, points);

    ProjectToVtk toVtk("IndexTreeToSimplices3Test_Level1ToVtk");
    IndexTreeToSimplices3::cellsToVtkData(toVtk, tets);
    const auto *vtkMesh = toVtk.get().front();
    ASSERT_EQ(8 * ReferenceShapeCube::numTetsInStandardSplit, vtkMesh->getNumCells());
    ASSERT_EQ(27, vtkMesh->getNumNodes());
    // Paraview::WriteList(toVtk.get());

    const auto boundaryFaces = allTets.getBoundaryFaces();
    ASSERT_EQ(boundaryFaces.getNumTriangles(), 48);
    const Boundary1 bnd = Boundary1::createFromBoundaryEdges(boundaryFaces);
    ASSERT_TRUE(bnd.empty());
}

TEST(IndexTreeToSimplices3Test, GetCubeFromIndex)
{
    const IndexTree<GeomDim3> tree;
    const auto cube = IndexTreeToSimplices3::getCubeFromIndex(tree.getRoot());
    const auto *adjacencyC2E =
        *ReferenceShapeCube::getInstance().getAdjacencies().getAdjacency(Topology::Corner, Topology::Edge);
    for (int edge = 0; edge < Topology::NumEdgesOnCube; ++edge)
    {
        const auto &edgePoints = adjacencyC2E->getConnectedLowers(edge);
        ASSERT_EQ(edgePoints.size(), 2);
        ASSERT_EQ(PointUtils::GetL1Distance(cube[edgePoints[0]], cube[edgePoints[1]]), 1);
    }
}

TEST(IndexTreeToSimplices3Test, GetRefinedEdges1)
{
    IndexTree<GeomDim3> tree;
    RefineToMaxLevel<GeomDim3> doRefine1{1};
    tree.refineLeaves(doRefine1);
    const std::set<IndexTreeToSimplices3::TreeEdge> refinedEdges = IndexTreeToSimplices3::getRefinedEdges(tree);
    ASSERT_EQ(refinedEdges.size(), Topology::NumEdgesOnCube);
    for (const auto &edge : refinedEdges)
    {
        ASSERT_EQ(PointUtils::GetL1Distance(edge.point0, edge.point1), 1);
    }
}

TEST(IndexTreeToSimplices3Test, SingleRefinedCell)
{
    IndexTree<GeomDim3> tree;
    ContainsPoint containsOrigin{RatPoint3{0, 0, 0}};
    auto doRefine = [&containsOrigin](const Index<GeomDim3> &indx) {
        return indx.getLevel() < 1 || (indx.getLevel() < 2 && containsOrigin(indx));
    };
    tree.refineUntilReady(doRefine);
    ActionCount<GeomDim3> actionCount;
    tree.foreachLeaf(actionCount);
    ASSERT_EQ(actionCount(), 15);

    const auto tets = IndexTreeToSimplices3::Create(tree);
    ProjectToVtk toVtk("IndexTreeToSimplices3Test_SingleRefinedCell");
    IndexTreeToSimplices3::cellsToVtkData(toVtk, tets);
    // Paraview::WriteList(toVtk.get());
}
