#include "GraphIsomorphismUtils.h"

#include "GraphIsomorphismGrouper.h"
#include "GraphIsomorphismTaggerChains.h"
#include "GraphIsomorphismTaggerComponents.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "GraphIsomorphismTaggerMaxDegree.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "GraphIsomorphismTaggerTriangles.h"
#include "GraphIsomorphismConstruct.h"   // TODO remove

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

const std::vector<ITaggerFactory *> allFactories{&factoryNumbers, &factoryComponents, &factoryDegree,   &factoryKnown,
                                              &factoryChains,  &factoryMaxDegree,  &factoryDistance, &factoryTriangles};
}

std::vector<ITaggerFactory *> GraphIsomorphism::getTaggerFactories()
{
    return allFactories;
}



