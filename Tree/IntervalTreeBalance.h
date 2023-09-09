#pragma once

#include "IntervalTree.h"


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
