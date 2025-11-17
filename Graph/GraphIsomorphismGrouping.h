#pragma once

#include "Defines.h"

namespace GraphIsomorphism
{

template <typename T> class Grouping
{
  public:
    explicit Grouping(const std::vector<T> &values)
    {
        std::map<T, std::vector<T>> grouped;
        for (const auto &value : values)
        {
            grouped[value].push_back(value);
        }

        m_groups.resize(grouped.size());
        str::transform(grouped, m_groups.begin(), [](const auto &itr) { return itr.second; });
    }

    template <typename C> Grouping(const std::vector<T> &values, C compare)
    {
        std::map<T, std::vector<T>, C> grouped(compare);
        for (const auto &value : values)
        {
            grouped[value].push_back(value);
        }

        m_groups.resize(grouped.size());
        str::transform(grouped, m_groups.begin(), [](const auto &itr) { return itr.second; });
    }

    const std::vector<std::vector<T>> &operator()() const
    {
        return m_groups;
    }

    std::vector<size_t> getGroupSizes() const
    {
        std::vector<size_t> result(m_groups.size());
        str::transform(m_groups, result.begin(), [](const auto &g) { return g.size(); });
        return result;
    }

  private:
    std::vector<std::vector<T>> m_groups;
};

template <typename T, typename V> class TaggedGrouping
{
  public:
    template <typename P> TaggedGrouping(const std::vector<V> &values, P &tagProvider)
    {
        for (const auto &value : values)
        {
            m_groups[tagProvider(value)].push_back(value);
        }

        m_tags.resize(m_groups.size());
        str::transform(m_groups, m_tags.begin(), [](const auto &itr) { return itr.first; });
    }

    const std::vector<T> &getTags() const
    {
        return m_tags;
    }

    const std::vector<V> &getGroupMembers(const T &tag) const
    {
        const auto found = m_groups.find(tag);
        if (found != m_groups.end())
        {
            return found->second;
        }
        return m_emptyList;
    }

    std::vector<T> getUniqueTags() const
    {
        std::vector<T> result;
        str::copy_if(m_tags, std::back_inserter(result),
                     [this](const auto &tag) { return m_groups.at(tag).size() == 1; });
        return result;
    }

    size_t countUnique() const
    {
        return str::count_if(m_groups, [](const auto &itr) { return itr.second.size() == 1; });
    }

  private:
    std::vector<T> m_tags;
    std::map<T, std::vector<V>> m_groups;
    std::vector<V> m_emptyList;
};

} // namespace GraphIsomorphism
