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

std::pair<ChainId, Chain> ConstrutcChain(const UndirectedGraph &graph, const Chain &part1, Chain part2)
{
    auto retval = part1;
    str::reverse(retval);
    retval.insert(retval.end(), part2.begin(), part2.end());
    retval.erase(std::unique(retval.begin(), retval.end()), retval.end());

    if (retval.front() == retval.back())
    {
        // This is a cycle
        retval.pop_back();
        ChainId id = ChainId::AttachedCycle;
        if (graph.getDegree(retval.front()) == 2)
        {
            id = ChainId::PureCycle;
            std::rotate(retval.begin(), str::min_element(retval), retval.end());
        }
        // Orient
        if (retval.at(1) > retval.back())
        {
            std::reverse(retval.begin() + 1, retval.end());
        }
        return {id, retval};
    }
    else
    {
        // This is a path
        const auto degreeF = graph.getDegree(retval.front());
        const auto degreeB = graph.getDegree(retval.back());
        if (degreeF == 1 && degreeB == 1)
        {
            if (retval.front() > retval.back())
            {
                str::reverse(retval);
            }
            return {ChainId::PurePath, retval};
        }
        else if (degreeF == 1)
        {
            str::reverse(retval);
            return {ChainId::AttachedPathSingle, retval};
        }
        else if (degreeB == 1)
        {
            return {ChainId::AttachedPathSingle, retval};
        }
        else
        {
            if (retval.front() > retval.back())
            {
                str::reverse(retval);
            }
            return {ChainId::AttachedPathDouble, retval};
        }
    }
    throw MyException("Should not come here");
}

Chain GetChainPart(const UndirectedGraph &graph, GraphVertex vertex, GraphVertex current, std::set<GraphVertex> &done)
{
    Chain result{vertex};
    std::vector<GraphVertex> ngbs;
    while (graph.getDegree(current) == 2)
    {
        graph.setAdjacentVertices(current, ngbs);
        if (ngbs.front() != vertex && !done.contains(ngbs.front()))
        {
            result.push_back(current);
            done.insert(current);
            current = ngbs.front();
            continue;
        }
        else if (ngbs.back() != vertex && !done.contains(ngbs.back()))
        {
            result.push_back(current);
            done.insert(current);
            current = ngbs.back();
        }
        else
        {
            break;
        }
    }
    result.push_back(current);
    done.insert(current);
    return result;
}

std::pair<ChainId, Chain> GetChain(const UndirectedGraph &graph, GraphVertex vertex, std::set<GraphVertex> &done)
{
    std::vector<GraphVertex> ngbs;
    graph.setAdjacentVertices(vertex, ngbs);
    const Chain part1 = GetChainPart(graph, vertex, ngbs.front(), done);

    Chain part2{vertex};
    if (ngbs.size() == 2)
    {
        part2 = GetChainPart(graph, vertex, ngbs.back(), done);
    }

    return ConstrutcChain(graph, part1, part2);
}

std::map<TagEntry, std::vector<Chain>> GetChains(const UndirectedGraph &graph)
{
    std::map<TagEntry, std::vector<Chain>> retval;
    const auto nVertices = graph.getNumVertices();
    std::vector<GraphVertex> ngbs;
    std::set<GraphVertex> done;
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        const auto degree = graph.getDegree(v);
        if (degree == 0)
        {
            // Do nothing
        }
        else if (!done.contains(v) && degree <= 2)
        {
            const auto idAndChain = GetChain(graph, v, done);
            retval[idAndChain.first].push_back(idAndChain.second);
        }
        done.insert(v);
    }
    return retval;
}

std::vector<VertexTag> GenerateTags(const UndirectedGraph &graph)
{
    GetChains(graph);

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
