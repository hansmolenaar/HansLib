#pragma once

#include "IntervalTree.h"

// Refine such that there is no more than 1 level difference between adjacent cells
namespace IntervalTree
{
   template<int N>
   void Balance(IndexTree<N>& tree);

   template<>
   void Balance(IndexTree<1>& tree);

   template<>
   void Balance(IndexTree<2>& tree);

   template<>
   void Balance(IndexTree<3>& tree);
}
