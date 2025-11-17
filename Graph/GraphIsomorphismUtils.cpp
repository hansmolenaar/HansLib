#include "GraphIsomorphismUtils.h"

#include "Defines.h"
#include "GraphIsomorphismVertexGrouper.h"
#include "GraphIsomorphismTaggerChains.h"
#include "GraphIsomorphismTaggerComponents.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "GraphIsomorphismTaggerMaxDegree.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "GraphIsomorphismTaggerTriangles.h"

using namespace GraphIsomorphism;

template Tag GraphIsomorphism::CondenseSizeSequence(const std::vector<int> &sizes);
template Tag GraphIsomorphism::CondenseSizeSequence(const std::vector<size_t> &sizes);

template <typename T> Tag GraphIsomorphism::CondenseSizeSequence(const std::vector<T> &sizes)
{
    std::map<T, TagEntry> count;
    for (const auto siz : sizes)
    {
        count[siz] += 1;
    }

    Tag result;
    for (const auto &itr : count)
    {
        result.push_back(itr.first);
        result.push_back(itr.second);
    }

    return result;
}

std::ostream &operator<<(std::ostream &os, const GraphIsomorphism::Tag &tag)
{
    bool first = true;
    os << "{";
    for (TagEntry entry : tag)
    {
        if (!first)
        {
            os << ",";
        }
        first = false;
        os << " " << entry;
    }
    os << " }";
    return os;
}

std::ostream &operator<<(std::ostream &os, const GraphIsomorphism::GraphTags &tags)
{
    os << "{";
    for (const auto &tag : tags)
    {
        os << " " << tag;
    }
    os << " }";
    return os;
}

namespace
{
TaggerNumbersFactory factoryNumbers;
TaggerComponentFactory factoryComponents;
TaggerDegreeFactory factoryDegree;
TaggerKnownFactory factoryKnown;
TaggerChainsFactory factoryChains;
TaggerMaxDegreeFactory factoryMaxDegree;
TaggerDistanceFactory factoryDistance;
TaggerTrianglesFactory factoryTriangles;

const std::vector<ITaggerFactory *> allFactories{&factoryNumbers,  &factoryComponents, &factoryDegree,
                                                 &factoryKnown,    &factoryChains,     &factoryMaxDegree,
                                                 &factoryDistance, &factoryTriangles};
} // namespace

std::vector<ITaggerFactory *> GraphIsomorphism::getTaggerFactories()
{
    return allFactories;
}

std::vector<std::unique_ptr<ITagger>> GraphIsomorphism::getAllTaggers(const Graph::IGraphUs &graph)
{
    std::vector<std::unique_ptr<ITagger>> result(getTaggerFactories().size());
    str::transform(getTaggerFactories(), result.begin(),
                   [&graph](const auto &factory) { return factory->createTagger(graph); });
    return result;
}

std::vector<const IGraphTagger *> GraphIsomorphism::selectGraphTaggers(
    const std::vector<std::unique_ptr<ITagger>> &taggers)
{
    std::vector<const IGraphTagger *> result(taggers.size());
    str::transform(taggers, result.begin(), [](const auto &tagger) { return tagger->getGraphTagger(); });
    result.erase(std::remove(result.begin(), result.end(), nullptr), result.end());
    return result;
}

std::vector<const IVertexTagger *> GraphIsomorphism::selectVertexTaggers(
    const std::vector<std::unique_ptr<ITagger>> &taggers)
{
    std::vector<const IVertexTagger *> result(taggers.size());
    str::transform(taggers, result.begin(), [](const auto &tagger) { return tagger->getVertexTagger(); });
    result.erase(std::remove(result.begin(), result.end(), nullptr), result.end());
    return result;
}
