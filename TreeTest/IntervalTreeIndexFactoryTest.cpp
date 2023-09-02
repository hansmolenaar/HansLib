#include <gtest/gtest.h>

#include "IntervalTreeIndex.h"

using namespace IntervalTree;

TEST(IntervalTreeIndexFactoryTest, Factory1)
{
   Index1Factory factory;

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


TEST(IntervalTreeIndexFactoryTest, Factory1_refine)
{
   Index1Factory factory;

   const auto* root = factory.getRoot();
   const auto kids = factory.refine(*root);
   ASSERT_EQ(kids[0]->getKey(), 1);
   ASSERT_EQ(kids[1]->getKey(), 2);
}


TEST(IntervalTreeIndexFactoryTest, IndexFactory)
{
   IndexFactory<1> factory;
   const Interval<Rational> intv(Rational(3, 16), Rational(4, 16));

   const auto index = factory.get({ intv });
   ASSERT_EQ(index.getLevel(), 4);

   const auto rv = index.getInterval(0);
   ASSERT_EQ(rv.getLower(), intv.getLower());
   ASSERT_EQ(rv.getUpper(), intv.getUpper());
}


TEST(IntervalTreeIndexFactoryTest, IndexFactoryRoot)
{
   IndexFactory<2> factory;
   const auto* root = factory.getRoot();
   ASSERT_EQ(root->getLevel(), 0);
   ASSERT_EQ(root->getMeasure(), Rational(1, 1));
}


TEST(IntervalTreeIndexFactoryTest, IndexFactorLevel2Ref)
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