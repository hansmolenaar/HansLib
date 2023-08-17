#pragma once

#include "Rational.h"
#include "Interval.h"
#include "IntervalTreeDefines.h"
#include "Defines.h"
#include "BoolContainer.h"

#include <boost/functional/hash.hpp>

#include <vector>
#include <array>
#include <unordered_map>
#include <functional>


namespace IntervalTree
{


   class Index1
   {
   public:
      using Key = int;

      Index1(const Interval<Rational>& interval);
      Level getLevel() const;
      int getPositionInLevel() const;
      Key getKey() const;
      const Interval<Rational>& getInterval() const;
      std::array<Index1, 2> refine() const;
      Rational getMeasure() const;
      std::string toString() const;

   private:
      Interval<Rational> m_interval;
      Level m_level;
      int m_positionInLevel;
      Key m_key;
   };

   class Index1FlyWeightFactory
   {
   public:
      Index1FlyWeightFactory();
      const Index1* operator()(Index1::Key key) const;
      Index1::Key add(const Index1& index1);
      Index1::Key add(const Interval<Rational>& interval);
      const Index1* getRoot() const;
      std::array<const Index1*, 2> refine(const Index1& toRefine);

   private:
      std::unordered_map<Index1::Key, Index1> m_cache;
   };

   template<int N>
   class Index
   {
   public:
      using Key = std::array<Index1::Key, N>;

      Index(std::array<Index1::Key, N> keys, Index1FlyWeightFactory& factory);
      Level getLevel() const;
      const Interval<Rational>& getInterval(int n) const;
      Key getKey() const;
      std::array<Key, 1 << N> refine() const;
      Rational getMeasure() const;
      std::string toString() const;

      std::array<std::array<Rational, N>, 1 << N> getVerticesInVtkOrder() const;

   private:
      Index1FlyWeightFactory& m_factory1;
      Key m_keys;
   };
}

namespace std
{
   template <int N>
   struct hash< std::array<IntervalTree::Index1::Key, N> > {
      size_t operator()(const std::array<IntervalTree::Index1::Key, N>& key) const noexcept
      {
         size_t result = 0;
         for (int n = 0; n < N; ++n)
         {
            boost::hash_combine(result, static_cast<size_t>(key[n]));
         }
         return result;
      }
   };
}

namespace IntervalTree
{
   template<int N>
   class IndexFactory
   {
   public:
      Index<N> get(const std::array<Interval<Rational>, N>& keys);
      Index<N> get(std::initializer_list<Interval<Rational>> keys);
      const Index<N>* addIfNew(typename const Index<N>::Key& key);
      std::array<const Index<N>*, 1 << N> refine(const Index<N>& toRefine);
      const Index<N>* getRoot();
      const Index<N>* operator()(typename const Index<N>::Key& key) const;
   private:
      Index1FlyWeightFactory m_factory;
      std::unordered_map<typename Index<N>::Key, Index<N>> m_cache;
   };



   // Implementation

   template<int N>
   Index<N>::Index(std::array<Index1::Key, N> keys, Index1FlyWeightFactory& factory) :
      m_factory1(factory), m_keys(std::move(keys))
   {
   }


   template<int N>
   const Interval<Rational>& Index<N>::getInterval(int n) const
   {
      return m_factory1(m_keys.at(n))->getInterval();
   }

   template<int N>
   Level Index<N>::getLevel() const
   {
      Level result = 0;
      for (auto key : m_keys)
      {
         result = std::max(result, m_factory1(key)->getLevel());
      }
      return result;
   }

   template<int N>
   Index<N>::Key Index<N>::getKey() const
   {
      return m_keys;
   }


   template<int N>
   Rational Index<N>::getMeasure() const
   {
      return std::accumulate(m_keys.begin(), m_keys.end(), Rational(1, 1),
         [this](const Rational& acc, Index1::Key key1) {
            return acc * m_factory1(key1)->getMeasure();
         }
      );
   }

   template<int N>
   std::string Index<N>::toString() const
   {
      std::string result = "(" + m_factory1(m_keys[0])->toString();
      for (int n = 1; n < N; ++n)
      {
         result += ", " + m_factory1(m_keys[n])->toString();
      }
      result += ")";
      return result;
   }

   template<int N>
   std::array<typename Index<N>::Key, 1 << N> Index<N>::refine() const
   {
      constexpr size_t TwoPowN = 1 << N;
      std::array<typename Index<N>::Key, TwoPowN> result;
      Index1::Key  ref[N][2];

      // Refine each of the indices
      for (int d = 0; d < N; ++d)
      {
         const Index1::Key key1 = m_keys[d];
         const  Index1* indx1 = m_factory1(key1);
         const auto kids = indx1->refine();

         m_factory1.add(kids[0]);
         m_factory1.add(kids[1]);

         ref[d][0] = kids[0].getKey();
         ref[d][1] = kids[1].getKey();
      }

      for (int kid = 0; kid < TwoPowN; ++kid)
      {
         auto bools = BoolContainerUtils::FromNumber(kid);
         str::reverse(bools);
         while (bools.size() < N)
         {
            bools.push_back(false);
         }
         Index<N>::Key key;
         for (size_t n = 0; n < N; ++n)
         {
            const bool b = bools.at(n);
            key[n] = ref[n][b ? 1 : 0];
         }
         result[kid] = key;
      }
      return result;
   }

   template<int N>
   std::array<std::array<Rational, N>, 1<<N> Index<N>::getVerticesInVtkOrder() const
   {
      throw MyException("Index<N>::getVerticesInVtkOrder() not implemented");
   }

   template<>
   std::array<std::array<Rational, 1>, 2> Index<1>::getVerticesInVtkOrder() const;

   template<>
   std::array<std::array<Rational, 2>, 4> Index<2>::getVerticesInVtkOrder() const;

   template<>
   std::array<std::array<Rational, 3>, 8> Index<3>::getVerticesInVtkOrder() const;


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
   std::array<const Index<N>*, 1 << N> IndexFactory<N>::refine(const Index<N>& toRefine)
   {
      if (!m_cache.contains(toRefine.getKey()))
      {
         throw MyException("IndexFactory<N>::refine unknown index specified");
      }
      const auto kids = toRefine.refine();
      std::array<const Index<N>*, 1 << N> result;
      size_t n = 0;
      for (const auto& k : kids)
      {
         result[n] = addIfNew(k);
         ++n;
      }
      return result;
   }
  

}