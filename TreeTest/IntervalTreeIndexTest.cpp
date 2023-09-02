#include <gtest/gtest.h>

#include "IntervalTreeIndexFactory.h"

using namespace IntervalTree;


TEST(IntervalTreeIndexTest, Index_basics)
{
   IndexFactory<2> factory;
   const Interval<Rational> intv0(Rational(7, 8), Rational(8, 8));
   const Interval<Rational> intv1(Rational(3, 4), Rational(8, 8));

   const auto index = factory.get(std::array<Interval<Rational>, 2>{ intv0, intv1 });
   ASSERT_EQ(index.getLevel(), 3);

   const auto rv0 = index.getInterval(0);
   ASSERT_EQ(rv0.getLower(), intv0.getLower());
   ASSERT_EQ(rv0.getUpper(), intv0.getUpper());

   const auto rv1 = index.getInterval(1);
   ASSERT_EQ(rv1.getLower(), intv1.getLower());
   ASSERT_EQ(rv1.getUpper(), intv1.getUpper());

   const std::string str = index.toString();
   ASSERT_EQ(str, "((7/8, 1), (3/4, 1))");
}


TEST(IntervalTreeIndexTest, Index_refine)
{
   IndexFactory<2> factory;
   const auto* root = factory.getRoot();
   const auto kids = factory.refine(*root);

   ASSERT_EQ(kids[0]->getInterval(0), Interval<Rational>({ 0,1 }, { 1,2 }));
   ASSERT_EQ(kids[0]->getInterval(1), Interval<Rational>({ 0,1 }, { 1,2 }));

   ASSERT_EQ(kids[1]->getInterval(0), Interval<Rational>({ 1,2 }, { 1,1 }));
   ASSERT_EQ(kids[1]->getInterval(1), Interval<Rational>({ 0,1 }, { 1,2 }));

   ASSERT_EQ(kids[2]->getInterval(0), Interval<Rational>({ 0,1 }, { 1,2 }));
   ASSERT_EQ(kids[2]->getInterval(1), Interval<Rational>({ 1,2 }, { 1,1 }));

   ASSERT_EQ(kids[3]->getInterval(0), Interval<Rational>({ 1,2 }, { 1,1 }));
   ASSERT_EQ(kids[3]->getInterval(1), Interval<Rational>({ 1,2 }, { 1,1 }));
}

TEST(IntervalTreeIndexTest, Index_unequalLevel)
{
   Index1Factory factory1;
   const auto* root = factory1.getRoot();
   const auto kids = factory1.refine(*root);

   ASSERT_ANY_THROW(Index<3>(std::array<Index1::Key, 3>{root->getKey(), kids[0]->getKey(), kids[1]->getKey()}, factory1));
}


TEST(IntervalTreeIndexTest, GetParent)
{
   Index1Factory factory;
   const auto* root = factory.getRoot();
   ASSERT_TRUE(root->isRoot());

   const auto kids = factory.refine(*root);
   const Index<2> index(std::array<Index1::Key, 2>{kids[0]->getKey(), kids[1]->getKey()}, factory);
   ASSERT_FALSE(index.isRoot());
   const auto parent = index.getParent();
   ASSERT_TRUE(parent.isRoot());
}