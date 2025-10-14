#include <gtest/gtest.h>

#include "IntervalTreeIndexFactory.h"

using namespace IntervalTree;


TEST(IntervalTreeIndexTest, Index_basics)
{
   IndexFactory<2> factory;
   const Interval<Rational> intv0(Rational(7, 8), Rational(8, 8));
   const Interval<Rational> intv1(Rational(0, 1), Rational(1, 8));
   const Index1::Key key0 = Index1::Create(intv0).getKey();
   const Index1::Key key1 = Index1::Create(intv1).getKey();

   const auto* index = factory.addIfNew({ key0, key1 });
   ASSERT_EQ(index->getLevel(), 3);
   ASSERT_FALSE(Index<2>::IsRoot(index->getKey()));

   const auto& rv0 = index->getInterval(0);
   ASSERT_EQ(rv0.getLower(), intv0.getLower());
   ASSERT_EQ(rv0.getUpper(), intv0.getUpper());

   const auto& rv1 = index->getInterval(1);
   ASSERT_EQ(rv1.getLower(), intv1.getLower());
   ASSERT_EQ(rv1.getUpper(), intv1.getUpper());

   std::ostringstream os;
   os << *index;
   ASSERT_EQ(os.str(), "((7/8, 1), (0, 1/8))");
}


TEST(IntervalTreeIndexTest, Index_refine)
{
   IndexFactory<2> factory;
   const auto* root = factory.getRoot();
   ASSERT_TRUE(Index<2>::IsRoot(root->getKey()));
   const auto kids = factory.refine(*root);

   ASSERT_FALSE(Index<2>::IsRoot(kids[0]->getKey()));
   ASSERT_FALSE(Index<2>::IsRoot(kids[1]->getKey()));

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
   ASSERT_TRUE(Index<2>::IsRoot(Index<2>::GetParent(index.getKey())));
}


TEST(IntervalTreeIndexTest, GetAdjacentInDirRoot)
{
   IndexFactory<2> factory;
   const auto* root = factory.getRoot();
   for (int dir : {0, 1})
   {
      for (bool usePos : {false, true})
      {
         const auto indx = root->getAdjacentInDir(AdjacentDirection{ dir, usePos });
         ASSERT_FALSE(indx);
      }
   }
}


TEST(IntervalTreeIndexTest, GetAdjacentInDir)
{
   IndexFactory<2> factory;
   Index1Factory& factory1 = factory.getFactory1();
   const auto* root = factory.getRoot();
   const auto kids = root->refine();
   const auto* kid = factory.addIfNew(kids[2]);
   std::ostringstream os;
   os << *kid;
   ASSERT_EQ(os.str(), "((0, 1/2), (1/2, 1))");

   auto retval = kid->getAdjacentInDir(AdjacentDirection{ 0, false });
   ASSERT_FALSE(retval);

   retval = kid->getAdjacentInDir(AdjacentDirection{ 0, true });
   os.str("");
   os << Index<2>(*retval, factory1);
   ASSERT_EQ(os.str(), "((1/2, 1), (1/2, 1))");

   retval = kid->getAdjacentInDir(AdjacentDirection{ 1, false });
   os.str("");
   os << Index<2>(*retval, factory1);
   ASSERT_EQ(os.str(), "((0, 1/2), (0, 1/2))");

   retval = kid->getAdjacentInDir(AdjacentDirection{ 1, true });
   ASSERT_FALSE(retval);
}


TEST(IntervalTreeIndexTest, getBbOfCell1)
{
   IndexFactory<1> factory;
   const auto* root = factory.getRoot();
   const auto kids = root->refine();
   const auto* kid1 = factory.addIfNew(kids[1]);
   const auto bb = kid1->getBbOfCell();
   ASSERT_EQ(bb.getLower(0), Rational(1, 2));
   ASSERT_EQ(bb.getUpper(0), Rational(1, 1));
}


TEST(IntervalTreeIndexTest, getBbOfCell2)
{
   IndexFactory<2> factory;
   const auto* root = factory.getRoot();
   const auto kids = root->refine();
   const auto* kid1 = factory.addIfNew(kids[1]);
   const auto bb = kid1->getBbOfCell();
   ASSERT_EQ(bb.getLower(0), Rational(1, 2));
   ASSERT_EQ(bb.getLower(1), Rational(0, 2));
   ASSERT_EQ(bb.getUpper(0), Rational(2, 2));
   ASSERT_EQ(bb.getUpper(1), Rational(1, 2));
}
