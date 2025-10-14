#include <gtest/gtest.h>

#include "IntervalTreeIndex.h"

using namespace IntervalTree;

namespace
{
   void Check(const Interval<Rational>& intv, Level expectLevel, int expectPositionInLevel, Index1::Key expectKey)
   {
      const Index1 indx = Index1::Create(intv);
      ASSERT_EQ(indx.getLevel(), expectLevel);
      ASSERT_EQ(indx.getPositionInLevel(), expectPositionInLevel);
      ASSERT_EQ(indx.getKey(), expectKey);
   }
}


TEST(IntervalTreeIndex1Test, Level0)
{
   Check(Interval<Rational>(Rational(0, 10), Rational(10, 10)), 0, 0, 0);
}


TEST(IntervalTreeIndex1Test, Level1)
{
   Check(Interval<Rational>(Rational(0, 1), Rational(1, 2)), 1, 0, 1);
   Check(Interval<Rational>(Rational(1, 2), Rational(2, 2)), 1, 1, 2);
}


TEST(IntervalTreeIndex1Test, Level2)
{
   Check(Interval<Rational>(Rational(0, 2), Rational(1, 4)), 2, 0, 3);
   Check(Interval<Rational>(Rational(1, 4), Rational(1, 2)), 2, 1, 4);
   Check(Interval<Rational>(Rational(2, 4), Rational(3, 4)), 2, 2, 5);
   Check(Interval<Rational>(Rational(3, 4), Rational(4, 4)), 2, 3, 6);
}


TEST(IntervalTreeIndex1Test, Level3)
{
   Check(Interval<Rational>(Rational(0, 2), Rational(1, 8)), 3, 0, 7);
   Check(Interval<Rational>(Rational(1, 8), Rational(2, 8)), 3, 1, 8);
   Check(Interval<Rational>(Rational(2, 8), Rational(3, 8)), 3, 2, 9);
   Check(Interval<Rational>(Rational(3, 8), Rational(4, 8)), 3, 3, 10);

   Check(Interval<Rational>(Rational(4, 8), Rational(5, 8)), 3, 4, 11);
   Check(Interval<Rational>(Rational(5, 8), Rational(6, 8)), 3, 5, 12);
   Check(Interval<Rational>(Rational(6, 8), Rational(7, 8)), 3, 6, 13);
   Check(Interval<Rational>(Rational(7, 8), Rational(8, 8)), 3, 7, 14);
}


TEST(IntervalTreeIndex1Test, Index1ToString)
{
   const Index1 indx = Index1::Create(Interval<Rational>(Rational(0, 2), Rational(1, 4)));
   std::ostringstream os;
   os << indx;
   ASSERT_EQ(os.str(), "(0, 1/4)");
}
TEST(IntervalTreeIndex1Test, Index1Refine)
{
   const Interval<Rational> intv(Rational(2, 4), Rational(3, 4));
   const Index1 indx = Index1::Create(intv);
   const auto refined = indx.refine();
   ASSERT_EQ(refined[0].getInterval().getLower(), Rational(1, 2));
   ASSERT_EQ(refined[0].getInterval().getUpper(), Rational(5, 8));
   ASSERT_EQ(refined[1].getInterval().getLower(), Rational(5, 8));
   ASSERT_EQ(refined[1].getInterval().getUpper(), Rational(3, 4));
   ASSERT_FALSE(refined[0].isRoot());
   ASSERT_FALSE(refined[1].isRoot());
}

TEST(IntervalTreeIndex1Test, UnhappyPaths)
{
   ASSERT_ANY_THROW(Index1::Create(Interval<Rational>(Rational(-1, 4), Rational(0, 2))));
   ASSERT_ANY_THROW(Index1::Create(Interval<Rational>(Rational(1, 4), Rational(3, 4))));
   ASSERT_ANY_THROW(Index1::Create(Interval<Rational>(Rational(3, 5), Rational(4, 5))));
}


TEST(IntervalTreeIndex1Test, CreateRoot)
{
   const Index1 root = Index1::CreateRoot();
   ASSERT_TRUE(root.isRoot());
   ASSERT_TRUE(Index1::IsRoot(root.getKey()));
   ASSERT_EQ(root.getLevel(), 0);
   ASSERT_EQ(Index1::GetLevelFromKey(root.getKey()), root.getLevel());
   ASSERT_EQ(root.getPositionInLevel(), 0);
   ASSERT_EQ(root.getMeasure(), Rational(1, 1));
}


TEST(IntervalTreeIndex1Test, GetCenter)
{
   const Index1 root = Index1::CreateRoot();
   ASSERT_EQ(root.getCenter(), Rational(1, 2));

   const auto kids = root.refine();
   ASSERT_EQ(kids[0].getCenter(), Rational(1, 4));
   ASSERT_EQ(kids[1].getCenter(), Rational(3, 4));
}



TEST(IntervalTreeIndex1Test, getSibling)
{
   const Index1 root = Index1::CreateRoot();
   ASSERT_ANY_THROW(root.getSibling());

   const auto kids = root.refine();
   ASSERT_EQ(kids[0].getSibling().getPositionInLevel(), 1);
   ASSERT_EQ(kids[1].getSibling().getPositionInLevel(), 0);

   const auto kids0 = kids[0].refine();
   ASSERT_EQ(kids0[0].getSibling().getPositionInLevel(), 1);
   ASSERT_EQ(kids0[1].getSibling().getPositionInLevel(), 0);
   ASSERT_EQ(kids0[0].getCenter(), Rational(1, 8));
   ASSERT_EQ(kids0[1].getCenter(), Rational(3, 8));
   ASSERT_EQ(kids0[0].getSibling().getCenter(), Rational(3, 8));
   ASSERT_EQ(kids0[1].getSibling().getCenter(), Rational(1, 8));

   const auto kids1 = kids[1].refine();
   ASSERT_EQ(kids1[0].getSibling().getPositionInLevel(), 3);
   ASSERT_EQ(kids1[1].getSibling().getPositionInLevel(), 2);
   ASSERT_EQ(kids1[0].getCenter(), Rational(5, 8));
   ASSERT_EQ(kids1[1].getCenter(), Rational(7, 8));
   ASSERT_EQ(kids1[0].getSibling().getCenter(), Rational(7, 8));
   ASSERT_EQ(kids1[1].getSibling().getCenter(), Rational(5, 8));
}


TEST(IntervalTreeIndex1Test, getSiblingInDir)
{
   const Index1 root = Index1::CreateRoot();
   for (bool dir : {false, true})
   {
      auto indx = root.getSiblingInDir(dir);
      ASSERT_FALSE(indx);
   }

   const auto kids = root.refine();

   auto retval = kids[0].getSiblingInDir(false);
   ASSERT_FALSE(retval);
   retval = kids[1].getSiblingInDir(true);
   ASSERT_FALSE(retval);

   retval = kids[0].getSiblingInDir(true);
   ASSERT_EQ(retval->getCenter(), Rational(3, 4));

   retval = kids[1].getSiblingInDir(false);
   ASSERT_EQ(retval->getCenter(), Rational(1, 4));
}


TEST(IntervalTreeIndex1Test, getAdjacentInDir)
{
   const Index1 root = Index1::CreateRoot();
   for (bool dir : {false, true})
   {
      auto indx = root.getAdjacentInDir(dir);
      ASSERT_FALSE(indx);
   }

   const auto kids = root.refine();

   auto retval = kids[0].getAdjacentInDir(false);
   ASSERT_FALSE(retval);

   retval = kids[0].getAdjacentInDir(true);
   ASSERT_EQ(retval->getCenter(), Rational(3, 4));

   retval = kids[1].getAdjacentInDir(true);
   ASSERT_FALSE(retval);

   retval = kids[1].getAdjacentInDir(false);
   ASSERT_EQ(retval->getCenter(), Rational(1, 4));

   const auto kids0 = kids[0].refine();
   const auto kids1 = kids[1].refine();

   retval = kids0[0].getAdjacentInDir(false);
   ASSERT_FALSE(retval);
   retval = kids0[0].getAdjacentInDir(true);
   ASSERT_EQ(retval->getCenter(), Rational(3, 8));

   retval = kids0[1].getAdjacentInDir(false);
   ASSERT_EQ(retval->getCenter(), Rational(1, 8));
   retval = kids0[1].getAdjacentInDir(true);
   ASSERT_EQ(retval->getCenter(), Rational(5, 8));

   retval = kids1[0].getAdjacentInDir(false);
   ASSERT_EQ(retval->getCenter(), Rational(3, 8));
   retval = kids1[0].getAdjacentInDir(true);
   ASSERT_EQ(retval->getCenter(), Rational(7, 8));

   retval = kids1[1].getAdjacentInDir(false);
   ASSERT_EQ(retval->getCenter(), Rational(5, 8));
   retval = kids1[1].getAdjacentInDir(true);
   ASSERT_FALSE(retval);
}


TEST(IntervalTreeIndex1Test, getParent)
{
   const Index1 root = Index1::CreateRoot();
   ASSERT_ANY_THROW(root.getParent());

   const auto kids = root.refine();
   for (const auto& k : kids)
   {
      ASSERT_TRUE(k.getParent().isRoot());
      ASSERT_EQ(k.getParentKey(), root.getKey());
   }

   const auto kids0 = kids[0].refine();
   const auto kids1 = kids[1].refine();

   for (const auto& k : kids0)
   {
      ASSERT_EQ(k.getParent().getKey(), kids[0].getKey());
      ASSERT_EQ(k.getParentKey(), kids[0].getKey());
   }
   for (const auto& k : kids1)
   {
      ASSERT_EQ(k.getParent().getKey(), kids[1].getKey());
      ASSERT_EQ(k.getParentKey(), kids[1].getKey());
   }
}


TEST(IntervalTreeIndex1Test, DecomposeKey)
{
   auto retval = Index1::decomposeKey(0);
   ASSERT_EQ(std::get<0>(retval), 0);
   ASSERT_EQ(std::get<1>(retval), 0);

   retval = Index1::decomposeKey(1);
   ASSERT_EQ(std::get<0>(retval), 1);
   ASSERT_EQ(std::get<1>(retval), 0);

   retval = Index1::decomposeKey(2);
   ASSERT_EQ(std::get<0>(retval), 1);
   ASSERT_EQ(std::get<1>(retval), 1);

   retval = Index1::decomposeKey(3);
   ASSERT_EQ(std::get<0>(retval), 2);
   ASSERT_EQ(std::get<1>(retval), 0);
   retval = Index1::decomposeKey(4);
   ASSERT_EQ(std::get<0>(retval), 2);
   ASSERT_EQ(std::get<1>(retval), 1);
   retval = Index1::decomposeKey(5);
   ASSERT_EQ(std::get<0>(retval), 2);
   ASSERT_EQ(std::get<1>(retval), 2);
   retval = Index1::decomposeKey(6);
   ASSERT_EQ(std::get<0>(retval), 2);
   ASSERT_EQ(std::get<1>(retval), 3);


   retval = Index1::decomposeKey(7);
   ASSERT_EQ(std::get<0>(retval), 3);
   ASSERT_EQ(std::get<1>(retval), 0);
}

TEST(IntervalTreeIndex1Test, DecomposeKey2)
{
   for (Index1::Key key = 0; key < 1 << 10; ++key)
   {
      const auto [level, pos] = Index1::decomposeKey(key);
      ASSERT_EQ(Index1::composeKey(level, pos), key);
      ASSERT_EQ(Index1::GetLevelFromKey(key), level);
   }
}


TEST(IntervalTreeIndex1Test, CreateRoundTrip)
{
   for (Index1::Key key = 0; key < 1 << 10; ++key)
   {
      const auto index1 = Index1::CreateFromKey(key);
      const auto retval = Index1::Create(index1.getInterval());
      ASSERT_EQ(retval.getKey(), key);
      ASSERT_EQ(index1, retval);
   }
}


TEST(IntervalTreeIndex1Test, GetParentKey)
{
   for (Index1::Key key = 0; key < 10; ++key)
   {
      const auto parent = Index1::CreateFromKey(key);
      const auto kids = parent.refine();
      ASSERT_EQ(Index1::GetParentKey(kids[0].getKey()), key);
      ASSERT_EQ(Index1::GetParentKey(kids[1].getKey()), key);
   }
}