#include "GraphIsomorphismTaggerChains.h"
#include "Defines.h"
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

std::vector<VertexTag> GenerateTags(const UndirectedGraph &graph)
{
    const auto nVertices = graph.getNumVertices();
    std::vector<VertexTag> retval(nVertices);
    std::vector<std::map<size_t, TagEntry>> attachedCycles(nVertices);
    std::vector<std::map<size_t, TagEntry>> attachedPathsSingle(nVertices);
    std::map<VertexPair, std::map<size_t, TagEntry>> attachedPathDouble;

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
            const auto siz = cycle.size();

            // Relate to the attachment vertices
            attachedCycles[cycle.front()][siz] += 1;
            const auto cycleId = attachedCycles[cycle.front()][siz];

            // cycle, number it
            for (size_t n = 1; n < siz; ++n)
            {
                // ( ChainId, cycle size, number of cycle with this size, position in cycle )
                retval[cycle.at(n)] = std::vector<TagEntry>{ChainId::AttachedCycle, static_cast<TagEntry>(siz), cycleId,
                                                            static_cast<TagEntry>(n)};
            }
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
            // Front attached path
            attachedPathsSingle[path.front()][siz] += 1;
            const auto pathId = attachedPathsSingle[path.front()][siz];

            for (size_t n = 1; n < siz; ++n)
            {
                retval[path.at(n)] = std::vector<TagEntry>{ChainId::AttachedPathSingle, static_cast<TagEntry>(siz),
                                                           pathId, static_cast<TagEntry>(n)};
            }
        }
        else if (graph.getDegree(path.front()) == 1 && graph.getDegree(path.back()) > 2)
        {
            // back attached path
            attachedPathsSingle[path.back()][siz] += 1;
            const auto pathId = attachedPathsSingle[path.back()][siz];
            for (size_t n = 1; n < siz; ++n)
            {
                const size_t pos = siz - n - 1;
                retval[path.at(pos)] = std::vector<TagEntry>{ChainId::AttachedPathSingle, static_cast<TagEntry>(siz),
                                                             pathId, static_cast<TagEntry>(n)};
            }
        }
        else
        {
            MyAssert(graph.getDegree(path.front()) > 2 && graph.getDegree(path.back() > 2));
            const VertexPair orderedEndPoints{std::min(path.front(), path.back()), std::max(path.front(), path.back())};
            attachedPathDouble[orderedEndPoints][siz] += 1;
            const auto pathId = attachedPathDouble[orderedEndPoints][siz];
            // double attached path, number it symmetrically
            for (size_t n = 1; n < siz - 1; ++n)
            {
                const auto possym = std::min(n, siz - n - 1);
                retval[path.at(n)] = std::vector<TagEntry>{ChainId::AttachedPathDouble, static_cast<TagEntry>(siz),
                                                           pathId, static_cast<TagEntry>(possym)};
            }
        }
    }

    for (size_t n = 0; n < nVertices; ++n)
    {
        // std::vector<std::map<size_t, TagEntry>> attachedCycles(nVertices);
        if (attachedCycles.at(n).empty())
            continue;
        for (const auto &itr : attachedCycles.at(n))
        {
            retval.at(n).push_back(ChainId::AttachedCycle);
            retval.at(n).push_back(itr.first);
            retval.at(n).push_back(itr.second);
        }
    }

    for (size_t n = 0; n < nVertices; ++n)
    {
        // std::vector<std::map<size_t, TagEntry>> attachedPathsSingle(nVertices);
        if (attachedPathsSingle.at(n).empty())
            continue;
        for (const auto &itr : attachedPathsSingle.at(n))
        {
            retval.at(n).push_back(ChainId::AttachedPathSingle);
            retval.at(n).push_back(itr.first);
            retval.at(n).push_back(itr.second);
        }
    }

    // std::map<VertexPair, std::map<size_t, TagEntry>> attachedPathDouble;
    for (const auto &itrVerticesMap : attachedPathDouble)
    {
        for (GraphVertex v : {itrVerticesMap.first.first, itrVerticesMap.first.second})
        {
            for (const auto &itr : itrVerticesMap.second)
            {
                retval.at(v).push_back(ChainId::AttachedPathDouble);
                retval.at(v).push_back(itr.first);
                retval.at(v).push_back(itr.second);
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
