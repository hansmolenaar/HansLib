#pragma once

#include "Rational.h"
#include "IntervalTreeIndex.h"
#include "Logger.h"
#include <map>

namespace IntervalTree
{

   template<size_t N>
   struct ActionCount
   {
      void operator()(const Index<N>&) { ++m_count; };
      int operator()() const { return m_count; };
      void reset() { m_count = 0; }
   private:
      int m_count = 0;
   };

   template<size_t N>
   struct ActionCountPerLevel
   {
      void operator()(const Index<N>& index) { Count[index.getLevel()] += 1; };
      std::map<int, int> Count;
   };

   template<size_t N>
   struct ActionCollectLeaves
   {
      void operator()(const Index<N>& index) { Leaves.push_back(&index); };
      std::vector<const Index<N>*> Leaves;
   };

   template<size_t N>
   struct ActionMaxLevel
   {
      void operator()(const Index<N>& index) { MaxLevel = std::max(MaxLevel, index.getLevel()); };
      int MaxLevel = -1;
   };


   template<size_t N>
   struct ActionLogLeaves
   {
      void operator()(const Index<N>& index) {
         std::ostringstream os;
         os << index.getBbOfCell();
         ActionLogger.logLine(os.str());
      };
      Logger& ActionLogger;
   };

}
