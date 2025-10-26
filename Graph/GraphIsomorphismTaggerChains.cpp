#include "Defines.h"
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
            for (size_t n = 1; n < siz; ++n)
            {
                // ( ChainId, cycle size, number of cycle with this size, position in cycle )
                retval[cycle.at(n)] =
                    std::vector<TagEntry>{ChainId::AttachedCycle, static_cast<TagEntry>(siz), static_cast<TagEntry>(n)};
            }
            // and relate to the attachment vertices
            const ChainTag chainTag{AttachedCycle, siz};
            attachedChainTags[cycle.front()].push_back(chainTag);
        }
    }

    std::map<size_t, TagEntry> purePathCount;
    for (const auto &path : cap.Paths)
    {
        const auto siz = path.size();
        if (graph.getDegree(path.front()) == 1 && graph.getDegree(path.back()) == 1)
        {
            // Pure path, number it
            purePathCount[siz] += 1;
            for (size_t n = 0; n < siz; ++n)
            {
                // ( ChainId, path size, number of path with this size, position in cycle )
                retval[path.at(n)] = std::vector<TagEntry>{ChainId::PurePath, static_cast<TagEntry>(siz),
                                                           purePathCount[siz], static_cast<TagEntry>(n)};
            }
        }
        else if (graph.getDegree(path.front()) > 2 && graph.getDegree(path.back()) == 1)
        {
            // Front attached path, number it
            for (size_t n = 1; n < siz; ++n)
            {
                retval[path.at(n)] = std::vector<TagEntry>{ChainId::AttachedPathSingle, static_cast<TagEntry>(siz),
                                                           static_cast<TagEntry>(n)};
            }
            const ChainTag chainTag{AttachedPathSingle, siz};
            attachedChainTags[path.front()].push_back(chainTag);
        }
        else if (graph.getDegree(path.front()) == 1 && graph.getDegree(path.back()) > 2)
        {
            // back attached path, number it
            for (size_t n = 1; n < siz; ++n)
            {
                const size_t pos = siz - n - 1;
                retval[path.at(pos)] = std::vector<TagEntry>{ChainId::AttachedPathSingle, static_cast<TagEntry>(siz),
                                                             static_cast<TagEntry>(n)};
            }
            const ChainTag chainTag{AttachedPathSingle, siz};
            attachedChainTags[path.back()].push_back(chainTag);
        }
        else
        {
            MyAssert(graph.getDegree(path.front()) > 2 && graph.getDegree(path.back() > 2));

            // double attached path, number it symmetrically
            for (size_t n = 1; n < siz - 1; ++n)
            {
                const auto possym = std::min(n, siz - n - 1);
                retval[path.at(n)] = std::vector<TagEntry>{ChainId::AttachedPathDouble, static_cast<TagEntry>(siz),
                                                           static_cast<TagEntry>(possym)};
            }
            const ChainTag chainTag{AttachedPathDouble, siz};
            attachedChainTags[path.front()].push_back(chainTag);
            attachedChainTags[path.back()].push_back(chainTag);
        }
    }

    for (size_t n = 0; n < nVertices; ++n)
    {
        if (attachedChainTags.at(n).empty())
        {
            continue;
        }

        str::sort(attachedChainTags.at(n));
        VertexTag tag;
        for (const auto ct : attachedChainTags.at(n))
        {
            tag.push_back(ct.first);
            tag.push_back(ct.second);
        }
        retval.at(n) = tag;
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
