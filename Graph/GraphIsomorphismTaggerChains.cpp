#include "GraphIsomorphismTaggerChains.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
using Chain = std::vector<GraphVertex>;

enum ChainId : TagEntry
{
    PureCycle,
    PurePath,
    AttachedCycle,
    AttachedPathSingle,
    AttachedPathDouble
};
using ChainTag = std::pair<ChainId, size_t>;

std::vector<VertexTag> GenerateTags(const UndirectedGraph &graph)
{
    const auto nVertices = graph.getNumVertices();
    std::vector<VertexTag> retval(nVertices);
    std::vector<std::vector<ChainTag>> attachedChainTags(nVertices);

    const auto cap = graph.SplitInCyclesAndPaths();
    std::map<size_t, TagEntry> pureCycleCount;
    for (const auto &cycle : cap.Cycles)
    {
        if (graph.getDegree(cycle.front()) == 2)
        {
            // This is a pure chain, number it
            const auto siz = cycle.size();
            pureCycleCount[siz] += 1;
            for (size_t n = 0; n < siz; ++n)
            {
                // ( ChainId, cycle size, number of cycle with this size, position in cycle )
                retval[cycle.at(n)] = std::vector<TagEntry>{ChainId::PureCycle, static_cast<TagEntry>(siz),
                                                            pureCycleCount[siz], static_cast<TagEntry>(n)};
            }
        }
        else
        {
            MyAssert(graph.getDegree(cycle.front()) > 2);
            // Attached cycle, number it
            const auto siz = cycle.size();
            for (size_t n = 0; n < siz; ++n)
            {
                // ( ChainId, cycle size, number of cycle with this size, position in cycle )
                retval[cycle.at(n)] = std::vector<TagEntry>{ChainId::AttachedCycle, static_cast<TagEntry>(siz),
                                                            static_cast<TagEntry>(n)};
            }
            // and relate to the attachment vertices
            const ChainTag chainTag{AttachedCycle, siz};
            attachedChainTags[cycle.front()].push_back(chainTag);
        }
    }

    std::map<size_t, TagEntry> purePathCount;
    for (const auto &path : cap.Paths)
    {
        if (graph.getDegree(path.front()) == 1 && graph.getDegree(path.back()) == 1)
        {
            // Pure path, number it
            const auto siz = path.size();
            purePathCount[siz] += 1;
            for (size_t n = 0; n < siz; ++n)
            {
                // ( ChainId, path size, number of path with this size, position in cycle )
                retval[path.at(n)] = std::vector<TagEntry>{ChainId::PurePath, static_cast<TagEntry>(siz),
                                                           purePathCount[siz], static_cast<TagEntry>(n)};
            }
        }
    }
    return retval;
}

} // namespace

TaggerChains::TaggerChains(const UndirectedGraph &graph) : m_graph(graph), m_tags(GenerateTags(graph))
{
}

const UndirectedGraph &TaggerChains::getGraph() const
{
    return m_graph;
}

VertexTag TaggerChains::getTag(GraphVertex v) const
{
    return m_tags.at(v);
}
