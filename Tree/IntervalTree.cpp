#include "IntervalTree.h"
#include "MyException.h"
#include "VtkData.h"
#include "IntervalTreeAdjacentDirection.h"

#include <sstream>
#include <cmath>

using namespace IntervalTree;

namespace
{
   template<int N>
   void markUnbalancedLeaves(const IndexTree<N>& tree, const Index<N>& indx, std::unordered_set<typename Index<N>::Key>& toRefine)
   {
      for (const auto adjDir : GetAdjacentNeighbors<N>())
      {
         const auto [exist, ngbKey] = indx.getAdjacentInDir(adjDir);
            if (exist)
            {
               const auto ngbExisting = tree.getExistingSelfOrAncestor(ngbKey);
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
}




template<>
void IndexTree<1>::balance()
{
   auto toRefine = RefineToBalance<1>(*this);
   while (!toRefine.empty())
   {
      auto predicate = [&toRefine](const Index<1>& indx) {return toRefine.contains(indx.getKey()); };
      refineLeaves(predicate);
      toRefine = RefineToBalance<1>(*this);
   }
}