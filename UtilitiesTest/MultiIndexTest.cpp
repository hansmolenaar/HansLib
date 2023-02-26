#include <gtest/gtest.h>

#include "MultiIndex.h"

TEST(MultiIndexTest, OneD)
{
   const auto mi = MultiIndex<int>::Create(std::vector<int>{2});
   ASSERT_EQ(mi.getNumDimensions(), 1);
   ASSERT_EQ(mi.getFlatSize(), 2);
   ASSERT_EQ(mi.at(0), 2);

   const auto multiplet = mi.toMultiplet(1);
   ASSERT_EQ(multiplet.size(), 1);
   ASSERT_EQ(multiplet.at(0), 1);
   ASSERT_EQ(mi.toFlat(multiplet), 1);
}


TEST(MultiIndexTest, Uno)
{
   const auto mi = MultiIndex<int>::Create(std::vector<int>{1, 1, 1, 1});
   ASSERT_EQ(mi.getNumDimensions(), 4);
   ASSERT_EQ(mi.getFlatSize(), 1);

   const auto multiplet = mi.toMultiplet(0);
   ASSERT_TRUE(std::ranges::equal(multiplet, std::vector<int>{0, 0, 0, 0}));
   ASSERT_EQ(mi.toFlat(multiplet), 0);
}

TEST(MultiIndexTest, TwoD)
{
   const auto mi = MultiIndex<int>::Create(std::vector<int>{2, 3});
   ASSERT_EQ(mi.getNumDimensions(), 2);
   ASSERT_EQ(mi.getFlatSize(), 6);
   ASSERT_EQ(mi.at(0), 2);
   ASSERT_EQ(mi.at(1), 3);

   const auto multiplet = mi.toMultiplet(5);
   ASSERT_TRUE(std::ranges::equal(multiplet, std::vector<int>{1, 2}));
   ASSERT_EQ(mi.toFlat(multiplet), 5);
}

TEST(MultiIndexTest, FourD)
{
   const auto mi = MultiIndex<int>::Create(std::vector<int>{2, 3, 5, 7});
   ASSERT_EQ(mi.getNumDimensions(), 4);
   ASSERT_EQ(mi.getFlatSize(), 210);
   for (int n = 0; n < mi.getFlatSize(); ++n)
   {
      const auto multiplet = mi.toMultiplet(n);
      ASSERT_EQ(mi.toFlat(multiplet), n);
   }
}


TEST(MultiIndexTest, UnhappyPath_Create)
{
   ASSERT_ANY_THROW(MultiIndex<int>::Create(std::vector<int>{}));
   ASSERT_ANY_THROW(MultiIndex<int>::Create(std::vector<int>{2, 3, 0, 4}));
   ASSERT_ANY_THROW(MultiIndex<int>::Create(std::vector<int>{2, 3, -1, 4}));
}


TEST(MultiIndexTest, UnhappyPath_ToMultiplet)
{
   const auto mi = MultiIndex<int>::Create(std::vector<int>{1, 2, 3});
   ASSERT_ANY_THROW(mi.toMultiplet(-1));
   ASSERT_ANY_THROW(mi.toMultiplet(6));
   ASSERT_ANY_THROW(mi.toMultiplet(7));
}


TEST(MultiIndexTest, UnhappyPath_ToFlat)
{
   const auto mi = MultiIndex<int>::Create(std::vector<int>{1, 2, 3});
   ASSERT_ANY_THROW(mi.toFlat(std::vector<int>{0}));
   ASSERT_ANY_THROW(mi.toFlat(std::vector<int>{0, -1, 0}));
   ASSERT_ANY_THROW(mi.toFlat(std::vector<int>{0, 0, 3}));
}

