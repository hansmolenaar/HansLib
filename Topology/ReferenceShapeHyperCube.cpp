#include "ReferenceShapeHyperCube.h"
#include "BinomialCoefficient.h"
#include "Defines.h"
#include "MultiIndex.h"
#include "MyException.h"
#include "ReferenceShape.h"
#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"

#include <set>

using namespace Topology;

namespace
{
class ReferenceShapeHyperCubeImpl : public IReferenceShapeHyperCube
{
  public:
    ReferenceShapeHyperCubeImpl(std::unique_ptr<ITopologicalAdjacencies> &&adjacencies)
        : m_shape(std::make_unique<ReferenceShape>(std::move(adjacencies)))
    {
    }
    const ITopologicalAdjacencies &getAdjacencies() const override
    {
        return m_shape->getAdjacencies();
    }

  private:
    std::unique_ptr<ReferenceShape> m_shape;
};

std::unique_ptr<ITopologicalAdjacency> CreateEdge2Corner(TopologyDimension dim)
{
    Utilities::MyAssert(dim > Edge);
    const auto mi = MultiIndex<int>::Create(std::vector<int>(dim, 2));
    std::set<std::pair<int, int>> edges;
    std::vector<int> indx(dim);
    for (auto crnr = 0; crnr < mi.getFlatSize(); ++crnr)
    {
        mi.toMultiplet(crnr, indx);
        for (TopologyDimension d = 0; d < dim; ++d)
        {
            // flip d-th bit
            indx.at(d) = (indx.at(d) ? 0 : 1);
            const auto flat = static_cast<int>(mi.toFlat(indx));
            edges.emplace(std::min(crnr, flat), std::max(crnr, flat));
            // flip back
            indx.at(d) = (indx.at(d) ? 0 : 1);
        }
    }

    std::map<int, std::vector<int>> edgeMap;
    int edgeId = 0;
    for (auto edge : edges)
    {
        edgeMap.emplace(edgeId, std::vector<int>{edge.first, edge.second});
        ++edgeId;
    }
    return TopologicalAdjacency::Create(Topology::Edge, edgeId, Topology::Corner, 1 << dim, edgeMap);
}

std::unique_ptr<ITopologicalAdjacency> CreateToEdge(const ITopologicalAdjacency &toCorner,
                                                    const ITopologicalAdjacency &edgeToCorner,
                                                    const std::vector<int> &counts)
{
    Utilities::MyAssert(toCorner.getDimensionLow() == Topology::Corner);
    Utilities::MyAssert(toCorner.getDimensionHigh() > Topology::Edge);
    Utilities::MyAssert(edgeToCorner.getDimensionLow() == Topology::Corner);
    Utilities::MyAssert(edgeToCorner.getDimensionHigh() == Topology::Edge);
    std::map<int, std::vector<int>> toEdge;
    for (auto id = 0; id < counts.at(toCorner.getDimensionHigh()); ++id)
    {
        toEdge.emplace(id, std::vector<int>{});
        const auto &cornersInHigh = toCorner.getConnectedLowers(id);
        std::set<int> corners;
        corners.insert(cornersInHigh.begin(), cornersInHigh.end());
        for (auto edgeId = 0; edgeId < counts.at(Topology::Edge); ++edgeId)
        {
            const auto &edge2corner = edgeToCorner.getConnectedLowers(edgeId);
            if (corners.contains(edge2corner.at(0)) && corners.contains(edge2corner.at(1)))
            {
                toEdge.at(id).push_back(edgeId);
            }
        }
    }
    return TopologicalAdjacency::Create(toCorner.getDimensionHigh(), counts.at(toCorner.getDimensionHigh()),
                                        Topology::Edge, counts.at(Topology::Edge), toEdge);
}

std::unique_ptr<ITopologicalAdjacency> CreateHyperFace2Corner(TopologyDimension dim)
{
    Utilities::MyAssert(dim > Topology::Edge);
    const int countHi = 2 * dim;
    const int countLo = 1 << dim;
    const auto mi = MultiIndex<int>::Create(std::vector<int>(dim, 2));
    std::map<int, std::vector<int>> faces;
    std::vector<int> multiplet(dim);
    for (TopologyDimension d = 0; d < dim; ++d)
    {
        std::vector<int> faceL;
        std::vector<int> faceH;
        for (auto crnr = 0; crnr < countLo; ++crnr)
        {
            mi.toMultiplet(crnr, multiplet);
            if (multiplet.at(d) == 0)
            {
                faceL.push_back(crnr);
            }
            else
            {
                faceH.push_back(crnr);
            }
        }
        std::ranges::sort(faceL);
        std::ranges::sort(faceH);
        faces.emplace(static_cast<int>(faces.size()), faceL);
        faces.emplace(static_cast<int>(faces.size()), faceH);
    }

    return TopologicalAdjacency::Create(dim - 1, countHi, Topology::Corner, countLo, faces);
}

std::vector<std::unique_ptr<ITopologicalAdjacency>> CreateAdjecencyList(TopologyDimension dim,
                                                                        const std::vector<int> &counts)
{
    std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacencyList;
    for (TopologyDimension d = 0; d < dim; ++d)
    {
        adjacencyList.emplace_back(TopologicalAdjacency::CreateTrivial(dim, d, counts.at(d)));
    }

    if (dim <= Topology::Edge)
        return adjacencyList;
    if (dim <= Topology::Volume)
    {
        adjacencyList.emplace_back(CreateHyperFace2Corner(dim));
        const auto hf2c = adjacencyList.size() - 1;
        if (dim == Topology::Face)
            return adjacencyList;
        adjacencyList.emplace_back(CreateEdge2Corner(dim));
        const auto e2c = adjacencyList.size() - 1;
        adjacencyList.emplace_back(CreateToEdge(*adjacencyList.at(hf2c), *adjacencyList.at(e2c), counts));
    }
    return adjacencyList;
}

std::unique_ptr<IReferenceShapeHyperCube> Create(TopologyDimension dim)
{
    std::vector<int> counts;
    for (TopologyDimension m = 0; m <= dim; ++m)
    {
        counts.push_back((1 << (dim - m)) * BinomialCoefficient::Get(dim, m));
    }

    auto adjacencies = TopologicalAdjacencies::Create(counts, CreateAdjecencyList(dim, counts));
    std::unique_ptr<IReferenceShapeHyperCube> referenceShape =
        std::make_unique<ReferenceShapeHyperCubeImpl>(std::move(adjacencies));
    return referenceShape;
}

std::map<TopologyDimension, std::unique_ptr<IReferenceShapeHyperCube>> Shapes;
} // namespace

const IReferenceShapeHyperCube &ReferenceShapeHyperCube::Get(TopologyDimension dim)
{
    if (!Shapes.contains(dim))
    {
        Shapes.emplace(dim, Create(dim));
    }
    return *Shapes.at(dim);
}