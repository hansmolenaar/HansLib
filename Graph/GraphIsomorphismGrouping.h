#pragma once

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

    const std::vector<std::vector<T>> get() const
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

} // namespace GraphIsomorphism
