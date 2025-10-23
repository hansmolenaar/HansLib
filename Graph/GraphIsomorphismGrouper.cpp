#include "Defines.h"
#include "GraphIsomorphismGrouper.h"
#include "UndirectedGraph.h"

using namespace GraphIsomorphism;

namespace
{
struct Tag2Group
{
    int operator()(GraphVertex v)
    {
        const auto retval = tag2group.try_emplace(tagger.getTag(v), tag2group.size());
        return (retval.first)->second;
    }

    const IGraphIsomorphismTagger &tagger;
    std::map<VertexTag, int> tag2group;
};
} // namespace

GraphGroup Grouper::operator()(const IGraphIsomorphismTagger &tagger)
{
    const auto &graph = tagger.getGraph();
    const auto nVertices = tagger.getGraph().getNumVertices();

    Tag2Group t2g{tagger};
    GraphGroup result(nVertices);
    str::transform(stv::iota(static_cast<GraphVertex>(0), nVertices), result.begin(), t2g);
    return result;
}
