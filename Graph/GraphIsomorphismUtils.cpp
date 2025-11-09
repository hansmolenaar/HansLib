#include "GraphIsomorphismUtils.h"

using namespace GraphIsomorphism;

template GraphIsomorphism::Tag GraphIsomorphismUtils::CondenseSizeSequence(const std::vector<int> &sizes);
template GraphIsomorphism::Tag GraphIsomorphismUtils::CondenseSizeSequence(const std::vector<size_t> &sizes);

template <typename T> GraphIsomorphism::Tag GraphIsomorphismUtils::CondenseSizeSequence(const std::vector<T> &sizes)
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
