#pragma once

#include "BoolContainer.h"
#include "BoundingBox.h"
#include "IntervalTreeAdjacentDirection.h"
#include "IntervalTreeIndex1Factory.h"
#include "MyAssert.h"
#include "StdHash.h"

#include <array>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace IntervalTree
{

   template<size_t N>
   class Index
   {
   public:
      using Key = std::array<Index1::Key, N>;

      struct KeyHasher 
      {
          std::size_t operator()(const Index<N>::Key& key) const 
          {
	      return ArrayHasher<Index1::Key, N>{}(key);
          }   
      };

      Index(Key keys, Index1Factory& factory);

      Level getLevel() const;
      const Interval<Rational>& getInterval(int n) const;
      // Subset of [0,1]^N
      BoundingBox<Rational, N> getBbOfCell() const;

      Key getKey() const;
      std::array<Key, IntervalTree::NumKids<N> > refine() const;
      Rational getMeasure() const;
      std::array<Rational, N> getCenter() const;

      static bool IsRoot(const Key& key);
      bool isRoot() const;

      static Key GetParent(const Key& key);
      Index<N> getParent() const;

      std::optional<Key> getAdjacentInDir(AdjacentDirection direction) const;

      std::array<std::array<Rational, N>, NumKids<N>> getVerticesInVtkOrder() const;

      friend std::ostream& operator<<(std::ostream& os, const Index& ind)
      {
         os << "(" << *(ind.m_factory1(ind.m_keys[0]));
         for (int n = 1; n < N; ++n)
         {
            os << ", " << *(ind.m_factory1(ind.m_keys[n]));
         }
         os << ")";
         return os;
      }

   private:
      Index1Factory& m_factory1;
      Key m_keys;
   };

   template<size_t N>
   struct ComparePointer
   {
      bool operator()(const Index<N>* lhs, const Index<N>* rhs)
      {
         const auto levelDif = lhs->getLevel() - rhs->getLevel();
         if (levelDif < 0) return true;
         else if (levelDif > 0) return false;
         return lhs->getKey() < rhs->getKey();
      };
   };
}

namespace IntervalTree
{


   // Implementation

   template<size_t N>
   Index<N>::Index(Key keys, Index1Factory& factory) :
      m_factory1(factory), m_keys(std::move(keys))
   {
      // Make sure that m_factory1 contains all keys
      for (auto key : m_keys)
      {
         m_factory1.addIfNew(key);
      }

      // Check if all of same level
      const Level level = getLevel();
      if (!str::all_of(keys, [level, &factory](Index1::Key key) { return factory(key)->getLevel() == level; }))
      {
         throw MyException("Non-uniform refinement not yet implemented");
      }
   }


   template<size_t N>
   const Interval<Rational>& Index<N>::getInterval(int n) const
   {
      return m_factory1(m_keys.at(n))->getInterval();
   }

   template<size_t N>
   Level Index<N>::getLevel() const
   {
      Level result = 0;
      for (auto key : m_keys)
      {
         const auto [level, pos] = Index1::decomposeKey(key);
         result = std::max(result, level);
      }
      return result;
   }

   template<size_t N>
   Index<N>::Key Index<N>::getKey() const
   {
      return m_keys;
   }


   template<size_t N>
   Rational Index<N>::getMeasure() const
   {
      return std::accumulate(m_keys.begin(), m_keys.end(), Rational(1, 1),
         [this](const Rational& acc, Index1::Key key1) {
            return acc * m_factory1(key1)->getMeasure();
         }
      );
   }

   template<size_t N>
   std::array<typename Index<N>::Key, NumKids<N>> Index<N>::refine() const
   {
      std::array<typename Index<N>::Key, NumKids<N>> result;
      Index1::Key  ref[N][2];

      // Refine each of the indices
      for (int d = 0; d < N; ++d)
      {
         const Index1::Key key1 = m_keys[d];
         const  Index1* indx1 = m_factory1(key1);
         const auto kids = Index1::refine(key1);
         for (size_t n = 0; n < kids.size(); ++n)
         {
            ref[d][n] = m_factory1.addIfNew(kids.at(n))->getKey();
         }
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

   template<size_t N>
   bool Index<N>::IsRoot(const Index<N>::Key& key)
   {
      return str::all_of(key, Index1::IsRoot);
   }

   template<size_t N>
   bool Index<N>::isRoot() const
   {
      return getLevel() == 0;
   }

   template<size_t N>
   Index<N>::Key Index<N>::GetParent(const Index<N>::Key& key)
   {
      Utilities::MyAssert(!Index<N>::IsRoot(key));
      Index<N>::Key result;
      str::transform(key, result.begin(), Index1::GetParentKey);
      return result;
   }

   template<size_t N>
   Index<N> Index<N>::getParent() const
   {
      std::array<Index1::Key, N> parent;
      str::transform(m_keys, parent.begin(), [this](Index1::Key key) {return m_factory1.getParent(key)->getKey(); });
      return Index<N>(parent, m_factory1);
   }

   template<size_t N>
   std::optional<typename Index<N>::Key> Index<N>::getAdjacentInDir(AdjacentDirection direction) const
   {
      Index<N>::Key result = getKey();
      const Index1::Key key1 = result.at(direction.Direction);
      const auto adj = m_factory1(key1)->getAdjacentInDir(direction.UsePositiveDirection);
      if (!adj) return {};
      result[direction.Direction] = adj->getKey();
      return result;
   }

   template<size_t N>
   std::array<Rational, N> Index<N>::getCenter() const
   {
      std::array<Rational, N> result;
      str::transform(this->getKey(), result.begin(), [this](Index1::Key key) {return m_factory1(key)->getCenter(); });
      return result;
   }

   template<size_t N>
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
