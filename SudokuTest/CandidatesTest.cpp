#include <gtest/gtest.h>

#include "Candidates.h"
#include "StreamUtils.h"

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
   candidates.removeCandidate(4);
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

TEST(CandidatesTest, ToStringsDifferences)
{
   Candidates candidates;
   auto retval = Candidates::ToStringsDifferences(candidates);
   ASSERT_EQ(retval[0], "...");
   ASSERT_EQ(retval[1], "...");
   ASSERT_EQ(retval[2], "...");

   candidates.setOnly({9, 1});
   retval = Candidates::ToStringsDifferences(candidates);
   ASSERT_EQ(retval[0], "1..");
   ASSERT_EQ(retval[1], "...");
   ASSERT_EQ(retval[2], "..9");

   candidates.removeCandidate(1);
   retval = Candidates::ToStringsDifferences(candidates);
   ASSERT_EQ(retval[0], "...");
   ASSERT_EQ(retval[1], "...");
   ASSERT_EQ(retval[2], "..9");
}

TEST(CandidatesTest, ToStrings)
{
   Candidates candidates;

   auto retval = Candidates::ToStrings(candidates);
   ASSERT_EQ(retval[0], "123");
   ASSERT_EQ(retval[1], "456");
   ASSERT_EQ(retval[2], "789");

   candidates.setOnly({9, 1});
   retval = Candidates::ToStrings(candidates);
   ASSERT_EQ(retval[0], "1..");
   ASSERT_EQ(retval[1], "...");
   ASSERT_EQ(retval[2], "..9");

   candidates.removeCandidate(1);
   retval = Candidates::ToStrings(candidates);
   ASSERT_EQ(retval[0], "   ");
   ASSERT_EQ(retval[1], " 9 ");
   ASSERT_EQ(retval[2], "   ");
}

TEST(CandidatesTest, ContainsValues)
{
   Candidates candidates;
   const std::array<Value, 2> values{2, 8};
   ASSERT_TRUE(candidates.contains(values));
   candidates.removeCandidate(5);
   ASSERT_TRUE(candidates.contains(values));
   candidates.removeCandidate(2);
   ASSERT_FALSE(candidates.contains(values));
   candidates.removeCandidate(8);
   ASSERT_FALSE(candidates.contains(values));
}
