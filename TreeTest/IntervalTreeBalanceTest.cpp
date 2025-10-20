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
    const Statistics expect{1, {1}};
    ASSERT_EQ(GetStatistics(tree), expect);
}

TEST(IntervalTreeBalanceTest, BalanceTree1)
{
    IndexTree<1> tree;
    RefineIfContainsPoint<1> refinePoint{Rational{49, 100}};
    tree.refineLeaves(refinePoint);
    tree.refineLeaves(refinePoint);
    tree.refineLeaves(refinePoint);

    std::ostringstream os;
    os << GetStatistics(tree);
    ASSERT_EQ(os.str(), "7, {0, 1, 1, 2}");
    Balance(tree);
    os.str("");
    os << GetStatistics(tree);
    ASSERT_EQ(os.str(), "9, {0, 0, 3, 2}");
}

TEST(IntervalTreeBalanceTest, RefineAroundPoint2)
{
    constexpr int dim = 2;
    const std::array<Rational, dim> point{Rational{49, 100}, Rational{51, 100}};
    RefineIfContainsPoint<dim> refinePoint{point};
    RefineToMaxLevel<dim> refineToLevel{5};
    auto doRefine = [&refinePoint, &refineToLevel](const Index<dim> &indx) {
        return refinePoint(indx) && refineToLevel(indx);
    };

    IndexTree<dim> tree;
    tree.refineUntilReady(doRefine);

    const auto &statistics = GetStatistics(tree);
    const Statistics expect{21, {0, 3, 3, 3, 3, 4}};
    ASSERT_EQ(GetStatistics(tree), expect);

    Balance(tree);

    // Paraview::Write("IntervalTreeBalanceTest_RefineAroundPoint2", *GetVtkData(tree));
    std::ostringstream os;
    os << GetStatistics(tree);
    ASSERT_EQ(os.str(), "53, {0, 0, 12, 13, 11, 4}");
}

TEST(IntervalTreeBalanceTest, RefineAroundPoint3)
{
    constexpr int dim = GeomDim3;
    const std::array<Rational, dim> point{Rational{49, 100}, Rational{51, 100}, Rational{49, 100}};
    RefineIfContainsPoint<dim> refinePoint{point};
    RefineToMaxLevel<dim> refineToLevel{5};
    auto doRefine = [&refinePoint, &refineToLevel](const Index<dim> &indx) {
        return refinePoint(indx) && refineToLevel(indx);
    };

    IndexTree<dim> tree;
    tree.refineUntilReady(doRefine);

    const auto &statistics = GetStatistics(tree);
    std::ostringstream os;
    os << GetStatistics(tree);
    ASSERT_EQ(os.str(), "41, {0, 7, 7, 7, 7, 8}");

    Balance(tree);

    // Paraview::Write("IntervalTreeBalanceTest_RefineAroundPoint3", *GetVtkData(tree));
    os.str("");
    os << GetStatistics(tree);
    ASSERT_EQ(os.str(), "169, {0, 0, 57, 52, 31, 8}");
}
