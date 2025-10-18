#pragma once

#include "IntervalTree.h"

// Refine such that there is no more than 1 level difference between adjacent cells
namespace IntervalTree
{
   template<size_t N>
   void Balance(IndexTree<N>& tree);
}
