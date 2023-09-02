#include <gtest/gtest.h>

#include "IntervalTreeIndex.h"

using namespace IntervalTree;

TEST(IntervalTreeIndex1FactoryTest, Factory1)
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


TEST(IntervalTreeIndex1FactoryTest, Factory1_refine)
{
   Index1Factory factory;

   const auto* root = factory.getRoot();
   const auto kids = factory.refine(*root);
   ASSERT_EQ(kids[0]->getKey(), 1);
   ASSERT_EQ(kids[1]->getKey(), 2);
}


TEST(IntervalTreeIndex1FactoryTest, GetParent)
{
   Index1Factory factory;
   const auto root = factory.getRoot();
   ASSERT_EQ(factory.getParent(root->getKey()), nullptr);

   const Interval<Rational> intv(Rational(5, 8), Rational(6, 8));
   const Index1 indx(intv);
   factory.add(indx);

   auto parent = factory.getParent(indx);
   ASSERT_EQ(parent->getCenter(), Rational(5, 8));

   parent = factory.getParent(*parent);
   ASSERT_EQ(parent->getCenter(), Rational(3, 4));

   parent = factory.getParent(*parent);
   ASSERT_EQ(parent->getCenter(), Rational(1, 2));

   parent = factory.getParent(*parent);
   ASSERT_EQ(parent, nullptr);
}