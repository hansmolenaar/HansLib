#pragma once

#include "IntervalTreeIndex.h"
#include "StdHash.h"

namespace IntervalTree
{
template <size_t N> class IndexFactory
{
  public:
    IndexFactory();
    std::optional<const Index<N> *> get(const Index<N>::Key &key) const;

    const Index<N> *addIfNew(const Index<N>::Key &key);
    std::array<const Index<N> *, NumKids<N>> refine(const Index<N> &toRefine);
    const Index<N> *getRoot() const;

    // throws if non-existing
    const Index<N> *operator()(const Index<N>::Key &key) const;

    const Index1Factory &getFactory1() const;
    Index1Factory &getFactory1();

  private:
    Index1Factory m_factory;
    std::unordered_map<typename Index<N>::Key, Index<N>, typename Index<N>::KeyHasher> m_cache;
};

template <size_t N> const Index<N> *IndexFactory<N>::operator()(const Index<N>::Key &key) const
{
    return &m_cache.at(key);
}

template <size_t N> IndexFactory<N>::IndexFactory()
{
    typename Index<N>::Key key;
    str::fill(key, 0);
    addIfNew(key);
}

template <size_t N> const Index<N> *IndexFactory<N>::getRoot() const
{
    typename Index<N>::Key key;
    str::fill(key, 0);
    return (*this)(key);
}

template <size_t N> const Index<N> *IndexFactory<N>::addIfNew(const Index<N>::Key &key)
{
    if (!m_cache.contains(key))
    {
        m_cache.emplace(std::make_pair(key, Index<N>{key, m_factory}));
    }
    return &m_cache.at(key);
}

template <size_t N> std::array<const Index<N> *, NumKids<N>> IndexFactory<N>::refine(const Index<N> &toRefine)
{
    if (!m_cache.contains(toRefine.getKey()))
    {
        throw MyException("IndexFactory<N>::refine unknown index specified");
    }
    const auto kids = toRefine.refine();
    std::array<const Index<N> *, NumKids<N>> result;
    str::fill(result, nullptr);
    size_t n = 0;
    for (const auto &k : kids)
    {
        result[n] = addIfNew(k);
        ++n;
    }
    return result;
}

template <size_t N> const Index1Factory &IndexFactory<N>::getFactory1() const
{
    return m_factory;
}

template <size_t N> Index1Factory &IndexFactory<N>::getFactory1()
{
    return m_factory;
}

template <size_t N> std::optional<const Index<N> *> IndexFactory<N>::get(const Index<N>::Key &key) const
{
    const auto found = m_cache.find(key);
    if (found != m_cache.end())
    {
        return &found->second;
    }
    return {};
}
} // namespace IntervalTree
