#include "IntervalTreeBalance.h"

#include <unordered_set>

using namespace IntervalTree;

namespace
{
   template<size_t N>
   using IndexSet = std::unordered_set<typename Index<N>::Key, typename Index<N>::KeyHasher>;

   template<size_t N>
   void markUnbalancedLeaves(const IndexTree<N>& tree, const Index<N>& indx, IndexSet<N>& toRefine)
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

   template<size_t N>
   IndexSet<N> RefineToBalance(const IndexTree<N>& tree)
   {
      IndexSet<N>  result;
      auto action = [&result, &tree](const Index<N>& indx) {markUnbalancedLeaves(tree, indx, result); };
      tree.foreachLeaf(action);
      return result;
   }

   template<size_t N>
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
