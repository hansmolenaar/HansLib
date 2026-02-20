#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

namespace Graph
{
class UndirectedGraphDistance;
class UndirectedGraphTriangles;
} // namespace Graph

namespace GraphIsomorphism
{

struct ScalarVertexValues
{
    GraphIsomorphism::TagEntry Degree;
    GraphIsomorphism::TagEntry NumTriangles;
    GraphIsomorphism::TagEntry NumNeigborsInTriangles;
    auto operator<=>(const ScalarVertexValues &) const = default;
};

class TaggerDistance : public ICharacteristicsCompare, public IVertexCompare
{
  public:
    explicit TaggerDistance(std::shared_ptr<Graph::UndirectedGraphDistance>,
                            std::shared_ptr<Graph::UndirectedGraphTriangles>);
    explicit TaggerDistance(const Graph::IGraphUs &);
    const Tag &getGraphTag() const;
    std::weak_ordering compareCharacteristics(const ICharacteristicsCompare &) const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;
    const VertexGrouping &getVertexGrouping() const override;

  private:
    std::shared_ptr<Graph::UndirectedGraphDistance> m_distances;
    std::shared_ptr<Graph::UndirectedGraphTriangles> m_triangles;

    Tag m_graphTag;
    VertexGrouping m_vertexGrouping;
};

class CompareDistanceFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
