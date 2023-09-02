#include <gtest/gtest.h>

#include "IntervalTreeIndex.h"

using namespace IntervalTree;

TEST(IntervalTreeIndexTest, Factory1)
{
   Index1FlyWeightFactory factory;

   const Interval<Rational> intv(Rational(7, 8), Rational(8, 8));
   const Index1 indx(intv);
   const auto key = factory.add(indx);
   ASSERT_EQ(factory.add(intv), key);
   ASSERT_EQ(key, indx.getKey());
   const auto& found = factory(key);
   ASSERT_EQ(found->getLevel(), indx.getLevel());
   ASSERT_EQ(found->getPositionInLevel(), indx.getPositionInLevel());
   ASSERT_EQ(factory.getRoot()->getLevel(), 0);
   ASSERT_EQ(factory.getRoot()->getInterval().getLower(), Rational(0, 2));
   ASSERT_EQ(factory.getRoot()->getInterval().getUpper(), Rational(2, 2));
}


TEST(IntervalTreeIndexTest, Factory1_refine)
{
   Index1FlyWeightFactory factory;

   const auto* root = factory.getRoot();
   const auto kids = factory.refine(*root);
   ASSERT_EQ(kids[0]->getKey(), 1);
   ASSERT_EQ(kids[1]->getKey(), 2);
}


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

TEST(IntervalTreeIndexTest, IndexFactory)
{
   IndexFactory<1> factory;
   const Interval<Rational> intv(Rational(3, 16), Rational(4, 16));

   const auto index = factory.get({ intv });
   ASSERT_EQ(index.getLevel(), 4);

   const auto rv = index.getInterval(0);
   ASSERT_EQ(rv.getLower(), intv.getLower());
   ASSERT_EQ(rv.getUpper(), intv.getUpper());
}

TEST(IntervalTreeIndexTest, IndexFactoryRoot)
{
   IndexFactory<2> factory;
   const auto* root = factory.getRoot();
   ASSERT_EQ(root->getLevel(), 0);
   ASSERT_EQ(root->getMeasure(), Rational(1, 1));
}


TEST(IntervalTreeIndexTest, IndexFactorLevel2Ref)
{
   IndexFactory<3> factory;
   const auto* root = factory.getRoot();
   const auto level1 = factory.refine(*root);
   Rational sum = 0;
   for (const auto* l1 : level1)
   {
      for (const auto* l2 : factory.refine(*l1))
      {
         sum += l2->getMeasure();
      }
   }
   ASSERT_EQ(sum, Rational(1, 1));
}