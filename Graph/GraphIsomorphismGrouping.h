#pragma once

#include "Defines.h"

namespace GraphIsomorphism
{

template <typename T> class Grouping
{
  public:
    Grouping() = default;

    explicit Grouping(const auto &values)
    {
        std::map<T, std::vector<T>> grouped;
        for (const auto &value : values)
        {
            grouped[value].push_back(value);
        }

        m_groups.resize(grouped.size());
        str::transform(grouped, m_groups.begin(), [](const auto &itr) { return itr.second; });
    }

    template <typename C> Grouping(const auto &values, C compare)
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
        str::sort(result);
        return result;
    }

    std::vector<T> getUniqueValues() const
    {
        std::vector<T> result;
        for (const auto &itr : m_groups)
        {
            if (itr.size() == 1)
            {
                result.emplace_back(itr.front());
            }
        }
        return result;
    }

    size_t countUnique() const
    {
        return str::count_if(m_groups, [](const auto &itr) { return itr.size() == 1; });
    }

    size_t size() const
    {
        return std::accumulate(m_groups.begin(), m_groups.end(), 0UZ,
                               [](size_t acc, const std::vector<T> &g) { return acc + g.size(); });
    }

    static Grouping<T> Combine(const Grouping<T> &, const Grouping<T> &);

  private:
    std::vector<std::vector<T>> m_groups;
};

} // namespace GraphIsomorphism
