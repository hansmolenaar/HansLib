#include <gtest/gtest.h>

#include "IntervalTreeIndexFactory.h"

using namespace IntervalTree;

TEST(IntervalTreeIndexFactoryTest, IndexFactory)
{
   IndexFactory<1> factory;
   const Interval<Rational> intv(Rational(3, 16), Rational(4, 16));
   const Index1::Key key1= Index1::Create(intv).getKey();

   const auto* index = factory.addIfNew({ key1 });
   ASSERT_EQ(index->getLevel(), 4);

   const auto rv = index->getInterval(0);
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

TEST(IntervalTreeIndexFactoryTest, Get)
{
   IndexFactory<2> factory;
   const auto* root = factory.getRoot();
   const auto kids = root->refine();

   const auto getRoot = factory.get(root->getKey());
   ASSERT_TRUE(std::get<0>(getRoot));
   ASSERT_TRUE(std::get<1>(getRoot)->isRoot());

   const auto getKid = factory.get(kids[0]);
   ASSERT_FALSE(std::get<0>(getKid));
}