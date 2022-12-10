#include <gtest/gtest.h>

#include "Utilities/BinomialCoefficient.h"

TEST(BinomialCoefficientTest, Basics)
{
   ASSERT_EQ(1, BinomialCoefficient::Get(1, 0));
   ASSERT_EQ(1, BinomialCoefficient::Get(1, 1));
   ASSERT_EQ(1, BinomialCoefficient::Get(2, 0));
   ASSERT_EQ(2, BinomialCoefficient::Get(2, 1));
   ASSERT_EQ(2, BinomialCoefficient::Get(2, 1));
   ASSERT_EQ(6, BinomialCoefficient::Get(4, 2));
   ASSERT_EQ(1140, BinomialCoefficient::Get(20, 3));
   ASSERT_EQ(210, BinomialCoefficient::Get(10, 4));
}

TEST(BinomialCoefficientTest, TestError)
{
   ASSERT_ANY_THROW(BinomialCoefficient::Get(2, 4));
   ASSERT_ANY_THROW(BinomialCoefficient::Get(2, -1));
}