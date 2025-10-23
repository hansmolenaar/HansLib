#include "GraphIsomorphismGrouper.h"
#include "Defines.h"
#include "UndirectedGraph.h"

using namespace GraphIsomorphism;

GraphGroup Grouper::operator()(const IGraphIsomorphismTagger &tagger)
{
    const auto &graph = tagger.getGraph();
    const auto nVertices = graph.getNumVertices();
    std::vector<VertexTag> tags(nVertices);

    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        tags[v] = tagger.getTag(v);
    }

    std::map<VertexTag, int> tag2group;
    GraphGroup result(nVertices);
    str::transform(tags, result.begin(), [&tag2group](const auto &t) { return (tag2group[t] = tag2group.size()); });
    return result;
}
