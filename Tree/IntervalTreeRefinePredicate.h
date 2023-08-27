#pragma once

#include "Rational.h"
#include "IntervalTree.h"
#include "IntervalTreeIndex.h"
#include "Defines.h"


namespace IntervalTree
{

   template<int N>
   struct RefineToMaxLevel
   {
      bool operator()(const Index<N>& indx) const { return indx.getLevel() < MaxLevel; };
      int MaxLevel;
   };



   template<int N>
   struct RefineIfContainsPoint
   {
      bool operator()(const Index<N>& indx) const {
         return str::all_of(stv::iota(0, N), [this, &indx](int n) {return indx.getInterval(n).contains(Point[n]); });
      };
      std::array<Rational, N> Point;
   };


}
