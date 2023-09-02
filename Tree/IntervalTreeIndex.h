#pragma once

#include "IntervalTreeIndex1Factory.h"
#include "BoolContainer.h"

#include <boost/functional/hash.hpp>

#include <vector>
#include <array>
#include <unordered_map>
#include <functional>
#include <string>

namespace IntervalTree
{

   template<int N>
   class Index
   {
   public:
      using Key = std::array<Index1::Key, N>;

      Index(std::array<Index1::Key, N> keys, Index1Factory& factory);
      Level getLevel() const;
      const Interval<Rational>& getInterval(int n) const;
      Key getKey() const;
      std::array<Key, IntervalTree::NumKids<N> > refine() const;
      Rational getMeasure() const;
      std::string toString() const;

      std::array<std::array<Rational, N>, NumKids<N>> getVerticesInVtkOrder() const;

   private:
      Index1Factory& m_factory1;
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
  

   // Implementation

   template<int N>
   Index<N>::Index(std::array<Index1::Key, N> keys, Index1Factory& factory) :
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
   std::array<typename Index<N>::Key, NumKids<N>> Index<N>::refine() const
   {
      std::array<typename Index<N>::Key, NumKids<N>> result;
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

      for (int kid = 0; kid < NumKids<N>; ++kid)
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
   std::array<std::array<Rational, N>, NumKids<N>> Index<N>::getVerticesInVtkOrder() const
   {
      throw MyException("Index<N>::getVerticesInVtkOrder() not implemented");
   }

   template<>
   std::array<std::array<Rational, 1>, 2> Index<1>::getVerticesInVtkOrder() const;

   template<>
   std::array<std::array<Rational, 2>, 4> Index<2>::getVerticesInVtkOrder() const;

   template<>
   std::array<std::array<Rational, 3>, 8> Index<3>::getVerticesInVtkOrder() const;


  

}