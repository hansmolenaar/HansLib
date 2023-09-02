#pragma once

#include "IntervalTreeIndex.h"


namespace IntervalTree
{
   template<int N>
   class IndexFactory
   {
   public:
      Index<N> get(const std::array<Interval<Rational>, N>& keys);
      Index<N> get(std::initializer_list<Interval<Rational>> keys);
      const Index<N>* addIfNew(typename const Index<N>::Key& key);
      std::array<const Index<N>*, NumKids<N>> refine(const Index<N>& toRefine);
      const Index<N>* getRoot();
      const Index<N>* operator()(typename const Index<N>::Key& key) const;
   private:
      Index1Factory m_factory;
      std::unordered_map<typename Index<N>::Key, Index<N>> m_cache;
   };


   template<int N>
   inline Index<N> IndexFactory<N>::get(const std::array<Interval<Rational>, N>& intervals)
   {
      std::array<Index1::Key, N> keys;
      std::transform(intervals.begin(), intervals.end(), keys.begin(), [this](const Interval<Rational>& intv) {return m_factory.add(intv); });
      return Index<N>(keys, m_factory);
   }

   template<int N>
   Index<N> IndexFactory<N>::get(std::initializer_list<Interval<Rational>> intervals)
   {
      if (intervals.size() != N)
      {
         throw MyException("IndexFactory<N>::get(initalizer list) expected  size " + std::to_string(N) + ", actual: " + std::to_string(intervals.size()));
      }
      std::array<Index1::Key, N> keys;
      std::transform(intervals.begin(), intervals.end(), keys.begin(), [this](const Interval<Rational>& intv) {return m_factory.add(intv); });
      return Index<N>(keys, m_factory);
   }

   template<int N>
   const Index<N>* IndexFactory<N>::operator()(typename const Index<N>::Key& key) const
   {
      return &m_cache.at(key);
   }

   template<int N>
   const Index<N>* IndexFactory<N>::getRoot()
   {
      typename Index<N>::Key key;
      str::fill(key, 0);
      addIfNew(key);
      return (*this)(key);
   }

   template<int N>
   const Index<N>* IndexFactory<N>::addIfNew(typename const Index<N>::Key& key)
   {
      if (!m_cache.contains(key))
      {
         m_cache.emplace(std::make_pair(key, Index<N>{key, m_factory}));
      }
      return &m_cache.at(key);
   }

   template<int N>
   std::array<const Index<N>*, NumKids<N>> IndexFactory<N>::refine(const Index<N>& toRefine)
   {
      if (!m_cache.contains(toRefine.getKey()))
      {
         throw MyException("IndexFactory<N>::refine unknown index specified");
      }
      const auto kids = toRefine.refine();
      std::array<const Index<N>*, NumKids<N>> result;
      str::fill(result, nullptr);
      size_t n = 0;
      for (const auto& k : kids)
      {
         result[n] = addIfNew(k);
         ++n;
      }
      return result;
   }
}
