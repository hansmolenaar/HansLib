#pragma once

#include "IntervalTreeBalance.h"
#include <unordered_set>

using namespace IntervalTree;

namespace
{
   template<int N>
   void markUnbalancedLeaves(const IndexTree<N>& tree, const Index<N>& indx, std::unordered_set<typename Index<N>::Key>& toRefine)
   {
      for (const auto adjDir : GetAdjacentNeighbors<N>())
      {
         const auto  ngbKey = indx.getAdjacentInDir(adjDir);
         if (ngbKey)
         {
            const auto ngbExisting = tree.getExistingSelfOrAncestor(*ngbKey);
            if (indx.getLevel() - ngbExisting.getLevel() > 1)
            {
               toRefine.insert(ngbExisting.getKey());
            }
         }
      }
   }

   template<int N>
   std::unordered_set<typename Index<N>::Key> RefineToBalance(const IndexTree<N>& tree)
   {
      std::unordered_set<typename Index<N>::Key> result;
      auto action = [&result, &tree](const Index<N>& indx) {markUnbalancedLeaves(tree, indx, result); };
      tree.foreachLeaf(action);
      return result;
   }

   template<int N>
   void DoBalance(IndexTree<N>& tree)
   {
      auto toRefine = RefineToBalance<N>(tree);
      while (!toRefine.empty())
      {
         auto predicate = [&toRefine](const Index<N>& indx) {return toRefine.contains(indx.getKey()); };
         tree.refineLeaves(predicate);
         toRefine = RefineToBalance<N>(tree);
      }
   }

}

template<>
void IntervalTree::Balance(IndexTree<1>& tree)
{
   DoBalance(tree);
}

template<>
void IntervalTree::Balance(IndexTree<2>& tree)
{
   DoBalance(tree);
}

template<>
void IntervalTree::Balance(IndexTree<3>& tree)
{
   DoBalance(tree);
}