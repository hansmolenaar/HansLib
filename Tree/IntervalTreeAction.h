#pragma once

#include "Rational.h"
#include "IntervalTreeIndex.h"
#include <map>

namespace IntervalTree
{

   template<int N>
   struct ActionCount
   {
      void operator()(const Index<N>&) { ++m_count; };
      int operator()() const { return m_count; };
      void reset() { m_count = 0; }
   private:
      int m_count = 0;
   };

   template<int N>
   struct ActionCountPerLevel
   {
      void operator()(const Index<N>& index) { Count[index.getLevel()] += 1; };
      std::map<int, int> Count;
   };

   template<int N>
   struct ActionCollectLeaves
   {
      void operator()(const Index<N>& index) { Leaves.push_back(&index); };
      std::vector<const Index<N>*> Leaves;
   };

   template<int N>
   struct ActionMaxLevel
   {
      void operator()(const Index<N>& index) { MaxLevel = std::max(MaxLevel, index.getLevel()); };
      int MaxLevel = -1;
   };

}
