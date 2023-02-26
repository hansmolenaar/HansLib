#include <gtest/gtest.h>

#include "BoolContainer.h"

TEST(BoolContainerTest, Zero)
{
   const auto bools = BoolContainerUtils::FromNumber(0);
   const BoolContainer expect{ false };
   ASSERT_TRUE(std::ranges::equal(bools, expect));
   ASSERT_EQ(BoolContainerUtils::ToNumber(bools), 0);
}

TEST(BoolContainerTest, One)
{
   const auto bools = BoolContainerUtils::FromNumber(1);
   const BoolContainer expect{ true };
   ASSERT_TRUE(std::ranges::equal(bools, expect));
   ASSERT_EQ(BoolContainerUtils::ToNumber(bools), 1);
}


TEST(BoolContainerTest, Two)
{
   const auto bools = BoolContainerUtils::FromNumber(2);
   const BoolContainer expect{ true, false };
   ASSERT_TRUE(std::ranges::equal(bools, expect));
   ASSERT_EQ(BoolContainerUtils::ToNumber(bools), 2);
}


TEST(BoolContainerTest, Three)
{
   const auto bools = BoolContainerUtils::FromNumber(3);
   const BoolContainer expect{ true, true };
   ASSERT_TRUE(std::ranges::equal(bools, expect));
   ASSERT_EQ(BoolContainerUtils::ToNumber(bools), 3);
}


TEST(BoolContainerTest, Five)
{
   const auto bools = BoolContainerUtils::FromNumber(5);
   const BoolContainer expect{ true, false, true };
   ASSERT_TRUE(std::ranges::equal(bools, expect));
   ASSERT_EQ(BoolContainerUtils::ToNumber(bools), 5);
}


TEST(BoolContainerTest, RoundTrip)
{
   for (int n = 0; n < 100; ++n)
   {
      const auto bools = BoolContainerUtils::FromNumber(n);
      ASSERT_EQ(BoolContainerUtils::ToNumber(bools), n);
   }
}
