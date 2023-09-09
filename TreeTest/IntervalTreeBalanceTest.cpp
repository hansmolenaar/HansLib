#include <gtest/gtest.h>

#include "IntervalTreeBalance.h"
#include "IntervalTreeRefinePredicate.h"
#include "IntervalTreeStatistics.h"
#include "IntervalTreeVtk.h"
#include "Paraview.h"

using namespace IntervalTree;

TEST(IntervalTreeBalanceTest, BalanceRoot)
{
   IndexTree<1> tree;
   Balance(tree);
   const Statistics expect{ 1, {1} };
   ASSERT_EQ(GetStatistics(tree), expect);
}

TEST(IntervalTreeBalanceTest, BalanceTree1)
{
   IndexTree<1> tree;
   RefineIfContainsPoint<1> refinePoint{ Rational{49, 100} };
   tree.refineLeaves(refinePoint);
   tree.refineLeaves(refinePoint);
   tree.refineLeaves(refinePoint);

   ASSERT_EQ(GetStatistics(tree).toString(), "7, {0, 1, 1, 2}");
   Balance(tree);
   ASSERT_EQ(GetStatistics(tree).toString(), "9, {0, 0, 3, 2}");
}


TEST(IntervalTreeBalanceTest, RefineAroundPoint)
{
   constexpr int dim = 2;
   const  std::array<Rational, dim> point{ Rational{49, 100}, Rational{51, 100} };
   RefineIfContainsPoint<dim> refinePoint{ point };
   RefineToMaxLevel<dim> refineToLevel{ 5 };
   auto doRefine = [&refinePoint, &refineToLevel](const Index<dim>& indx) {return refinePoint(indx) && refineToLevel(indx); };

   IndexTree<dim> tree;
   tree.refineUntilReady(doRefine);

   const auto& statistics = GetStatistics(tree);
   const Statistics expect{ 21, {0, 3, 3, 3, 3, 4} };
   ASSERT_EQ(GetStatistics(tree), expect);

   Balance(tree);

   //Paraview::Write("IndexTreeTest_RefineAroundPoint", *GetVtkData(tree));
   ASSERT_EQ(GetStatistics(tree).toString(), "53, {0, 0, 12, 13, 11, 4}");
}

