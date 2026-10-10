#include <gtest/gtest.h>

#include "Candidates.h"

using namespace Sudoku;

TEST(CandidatesTest, DefaultConstructor)
{
   const Candidates candidates;
   ASSERT_FALSE(candidates.isSolved());
   ASSERT_EQ(candidates.size(), 9);
   ASSERT_TRUE(candidates.contains(1));
   ASSERT_TRUE(candidates.contains(9));
}

TEST(CandidatesTest, SetGiven)
{
   Candidates candidates;
   candidates.setOnly({4});
   ASSERT_TRUE(candidates.isSolved());
   ASSERT_EQ(candidates.size(), 1);
   ASSERT_TRUE(candidates.contains(4));
   ASSERT_FALSE(candidates.contains(5));
   ASSERT_EQ(candidates.tryGetSolvedValue().value(), 4);
}

TEST(CandidatesTest, Unset)
{
   Candidates candidates;
   ASSERT_TRUE(candidates.contains(4));
   candidates.unsetValue(4);
   ASSERT_FALSE(candidates.contains(4));

   ASSERT_FALSE(candidates.isSolved());
   ASSERT_EQ(candidates.size(), 8);
}

TEST(CandidatesTest, SetOnly)
{
   Candidates candidates;
   candidates.setOnly({1, 9, 1});
   ASSERT_TRUE(candidates.contains(1));
   ASSERT_TRUE(candidates.contains(9));
   ASSERT_FALSE(candidates.contains(8));
   ASSERT_TRUE(!candidates.tryGetSolvedValue());
   ASSERT_EQ(candidates.size(), 2);
   ASSERT_FALSE(candidates.isSolved());

   candidates.setOnly({9});
   ASSERT_TRUE(candidates.contains(9));
   ASSERT_FALSE(candidates.contains(1));
   ASSERT_TRUE(candidates.isSolved());

   ASSERT_ANY_THROW(candidates.setOnly({1}));
}
