#include <gtest/gtest.h>

#include "Assert.h"
#include "BoundsCheck.h"

using namespace Utilities;

TEST(BoundsCheckTest, LowAndUp)
{
   const auto bounds = BoundsCheck<int>::Create(1, 3);
   ASSERT_FALSE(bounds(0));
   ASSERT_TRUE(bounds(1));
   ASSERT_TRUE(bounds(2));
   ASSERT_TRUE(bounds(3));
   ASSERT_FALSE(bounds(4));
}

TEST(BoundsCheckTest, OnlyLow)
{
   const auto bounds = BoundsCheck<int>::CreateLowerBound(1);
   ASSERT_FALSE(bounds(0));
   ASSERT_TRUE(bounds(1));
   ASSERT_TRUE(bounds(2));
}

TEST(BoundsCheckTest, OnlyUp)
{
   const auto bounds = BoundsCheck<int>::CreateUpperBound(1);
   ASSERT_TRUE(bounds(0));
   ASSERT_TRUE(bounds(1));
   ASSERT_FALSE(bounds(2));
}

TEST(BoundsCheckTest, CreationFailure)
{
   ASSERT_MYEXCEPTION_MESSAGE(BoundsCheck<int>::Create(2, 1), "Assertion failure");
}


TEST(BoundsCheckTest, Default)
{
   const BoundsCheck<long> checker;
   ASSERT_TRUE(checker(-1));
   ASSERT_TRUE(checker(0));
   ASSERT_TRUE(checker(1));
}