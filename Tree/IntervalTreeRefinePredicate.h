#pragma once

#include "Rational.h"
#include "IntervalTree.h"
#include "IntervalTreeIndex.h"


namespace IntervalTree
{

   template<int N>
   struct RefineToMaxLevel
   {
      bool operator()(const Index<N>& indx) const { return indx.getLevel() < MaxLevel; };
      int MaxLevel;
   };


}
