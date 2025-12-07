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

class TaggerDistance : public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerDistance(std::shared_ptr<Graph::UndirectedGraphDistance>,
                            std::shared_ptr<Graph::UndirectedGraphTriangles>);
    explicit TaggerDistance(const Graph::IGraphUs &);
    const Tag &getVertexTag(Graph::Vertex) const;
    const Tag &getGraphTag() const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;

  private:
    std::shared_ptr<Graph::UndirectedGraphDistance> m_distances;
    std::shared_ptr<Graph::UndirectedGraphTriangles> m_triangles;

    Tag getTag(Graph::Vertex v) const;
    std::vector<Tag> m_tags;
    Tag m_graphTag;
};

class TaggerDistanceFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
