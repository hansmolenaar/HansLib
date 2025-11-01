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

struct ChainTag
{
    ChainId id;
    size_t size;
    GraphVertex attacheFirst = 0;
    GraphVertex attacheLast = 0;
    auto operator<=>(const ChainTag &) const = default;
    bool operator==(const ChainTag &) const = default;
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
        if (ngbs.front() != vertex && (graph.getDegree(ngbs.front()) > 2 || !done.contains(ngbs.front())))
        {
            result.push_back(current);
            done.insert(current);
            current = ngbs.front();
            continue;
        }
        else if (ngbs.back() != vertex && (graph.getDegree(ngbs.back()) > 2 || !done.contains(ngbs.back())))
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

std::vector<std::pair<ChainTag, Chain>> GetChains(const UndirectedGraph &graph)
{
    std::vector<std::pair<ChainTag, Chain>> retval;
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
            const auto &chain = idAndChain.second;
            retval.emplace_back(
                std::make_pair(ChainTag{idAndChain.first, chain.size(), chain.front(), chain.back()}, chain));
        }
        done.insert(v);
    }
    str::sort(retval);
    return retval;
}

std::vector<Tag> GenerateTags(const UndirectedGraph &graph)
{
    const auto nVertices = graph.getNumVertices();
    std::vector<Tag> retval(nVertices);

    const auto chains = GetChains(graph);
    auto currentItr = chains.begin();

    const auto itrPureCycles = std::find_if_not(
        currentItr, chains.end(), [](const auto &tagChain) { return tagChain.first.id == ChainId::PureCycle; });
    while (currentItr != itrPureCycles)
    {
        const TagEntry currentSize = currentItr->first.size;
        const auto itrSameSize = std::find_if_not(currentItr, itrPureCycles, [currentSize](const auto &tagChain) {
            return tagChain.first.size == currentSize;
        });
        TagEntry count = 1;
        for (; currentItr != itrSameSize; ++currentItr, ++count)
        {
            for (TagEntry n = 0; n < currentSize; ++n)
            {
                // ( ChainId, cycle size, number of cycle with this size, position in cycle )
                retval[currentItr->second.at(n)] = std::vector<TagEntry>{ChainId::PureCycle, currentSize, count, n};
            }
        }
    }

    const auto itrPurePath = std::find_if_not(
        currentItr, chains.end(), [](const auto &tagChain) { return tagChain.first.id == ChainId::PurePath; });
    while (currentItr != itrPurePath)
    {
        const TagEntry currentSize = currentItr->first.size;
        const auto itrSameSize = std::find_if_not(currentItr, itrPurePath, [currentSize](const auto &tagChain) {
            return tagChain.first.size == currentSize;
        });
        TagEntry count = 1;
        for (; currentItr != itrSameSize; ++currentItr, ++count)
        {
            for (TagEntry n = 0; n < currentSize; ++n)
            {
                // ( ChainId, path size, number of path with this size, position in path )
                retval[currentItr->second.at(n)] = std::vector<TagEntry>{ChainId::PurePath, currentSize, count, n};
            }
        }
    }

    const auto itrAttachedCycle = std::find_if_not(
        currentItr, chains.end(), [](const auto &tagChain) { return tagChain.first.id == ChainId::AttachedCycle; });
    while (currentItr != itrAttachedCycle)
    {
        const GraphVertex attachedTo = currentItr->first.attacheFirst;
        const auto itrSameAttachedTo =
            std::find_if_not(currentItr, itrAttachedCycle,
                             [attachedTo](const auto &tagChain) { return tagChain.first.attacheFirst == attachedTo; });
        while (currentItr != itrSameAttachedTo)
        {
            const TagEntry currentSize = currentItr->first.size;
            const auto itrSameSize =
                std::find_if_not(currentItr, itrSameAttachedTo,
                                 [currentSize](const auto &tagChain) { return tagChain.first.size == currentSize; });

            TagEntry count = 1;
            for (; currentItr != itrSameSize; ++currentItr, ++count)
            {
                for (TagEntry n = 1; n < currentSize; ++n)
                {
                    // ( ChainId, cycle size, number of cycle with this size, position in cycle )
                    retval[currentItr->second.at(n)] =
                        std::vector<TagEntry>{ChainId::AttachedCycle, currentSize, count, n};
                }
                const GraphVertex root = currentItr->first.attacheFirst;
                retval[root].push_back(ChainId::AttachedCycle);
                retval[root].push_back(currentSize);
            }
        }
    }

    const auto itrAttachedPathSingle = std::find_if_not(currentItr, chains.end(), [](const auto &tagChain) {
        return tagChain.first.id == ChainId::AttachedPathSingle;
    });
    while (currentItr != itrAttachedPathSingle)
    {
        const GraphVertex attachedTo = currentItr->first.attacheFirst;
        const auto itrSameAttachedTo =
            std::find_if_not(currentItr, itrAttachedPathSingle,
                             [attachedTo](const auto &tagChain) { return tagChain.first.attacheFirst == attachedTo; });
        while (currentItr != itrSameAttachedTo)
        {
            const TagEntry currentSize = currentItr->first.size;
            const auto itrSameSize =
                std::find_if_not(currentItr, itrSameAttachedTo,
                                 [currentSize](const auto &tagChain) { return tagChain.first.size == currentSize; });

            TagEntry count = 1;
            for (; currentItr != itrSameSize; ++currentItr, ++count)
            {
                for (TagEntry n = 1; n < currentSize; ++n)
                {
                    // ( ChainId, path size, number of paths with this size, position in path )
                    retval[currentItr->second.at(n)] =
                        std::vector<TagEntry>{ChainId::AttachedPathSingle, currentSize, count, n};
                }
                const GraphVertex root = currentItr->first.attacheFirst;
                retval[root].push_back(ChainId::AttachedPathSingle);
                retval[root].push_back(currentSize);
            }
        }
    }

    std::map<VertexPair, std::vector<TagEntry>> pairTags;
    const auto itrAttachedPathDouble = std::find_if_not(currentItr, chains.end(), [](const auto &tagChain) {
        return tagChain.first.id == ChainId::AttachedPathDouble;
    });
    while (currentItr != itrAttachedPathDouble)
    {
        const GraphVertex attachedTo = currentItr->first.attacheFirst;
        const auto itrSameAttachedTo =
            std::find_if_not(currentItr, itrAttachedPathDouble,
                             [attachedTo](const auto &tagChain) { return tagChain.first.attacheFirst == attachedTo; });
        while (currentItr != itrSameAttachedTo)
        {
            const TagEntry currentSize = currentItr->first.size;
            const auto itrSameSize =
                std::find_if_not(currentItr, itrSameAttachedTo,
                                 [currentSize](const auto &tagChain) { return tagChain.first.size == currentSize; });
            while (currentItr != itrSameSize)
            {
                const GraphVertex attachedOther = currentItr->first.attacheLast;
                const auto itrSameAttachedOther =
                    std::find_if_not(currentItr, itrSameSize, [attachedOther](const auto &tagChain) {
                        return tagChain.first.attacheLast == attachedOther;
                    });
                TagEntry count = 1;
                for (; currentItr != itrSameAttachedOther; ++currentItr, ++count)
                {
                    for (TagEntry n = 1; n < currentSize - 1; ++n)
                    {
                        const auto possym = std::min(n, currentSize - n - 1);
                        retval[currentItr->second.at(n)] =
                            std::vector<TagEntry>{ChainId::AttachedPathDouble, currentSize, count, possym};
                    }
                    pairTags[std::make_pair(currentItr->first.attacheFirst, currentItr->first.attacheLast)].push_back(
                        currentSize);
                }
            }
        }
    }

    std::map<GraphVertex, std::multiset<Tag>> perVertex;
    for (const auto &itrPair : pairTags)
    {
        Tag tag;
        for (const auto siz : itrPair.second)
        {
            tag.push_back(siz);
        }
        str::sort(tag);
        tag.insert(tag.begin(), ChainId::AttachedPathDouble);

        perVertex[itrPair.first.first].insert(tag);
        perVertex[itrPair.first.second].insert(tag);
    }

    for (const auto &itr : perVertex)
    {
        const GraphVertex v = itr.first;
        for (const auto &itrTag : itr.second)
        {
            retval[v].insert(retval[v].end(), itrTag.begin(), itrTag.end());
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

Tag TaggerChains::getTag(GraphVertex v) const
{
    return m_tags.at(v);
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ITagger> TaggerChainsFactory::create(const UndirectedGraph &graph)
{
    return std::make_unique<TaggerChains>(graph);
}
