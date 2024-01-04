#include <gtest/gtest.h>


#include "Rational.h"


TEST(RationalTest, DoesThisCompile)
{
   const Rational r2(1, 2);
   const Rational r3(1, 3);
   ASSERT_EQ(r2 - r3, Rational(1, 6));
   ASSERT_EQ(r2 + r3, Rational(5, 6));
   const auto r23 = r2 + Rational(1, 6);
   ASSERT_EQ(r23, Rational(2, 3));
}

TEST(RationalTest, Hash)
{
   const Rational p1{ 2, 3 };
   const size_t hash1 = std::hash<Rational>{}(p1);
   const Rational p2{ 3, 2 };
   const size_t hash2 = std::hash<Rational>{}(p2);
   ASSERT_NE(hash1, hash2);
}


TEST(RationalTest, ToFloat)
{
   const Rational frc{ 3, 4 };
   const double result = std::ToFloat<double>(frc);
   ASSERT_EQ(result, 0.75);
}


TEST(RationalTest, Abs)
{
   ASSERT_EQ(Rational(1, 2), std::abs(Rational(1, 2)));
   ASSERT_EQ(Rational(1, 2), std::abs(Rational(-1, 2)));
   ASSERT_EQ(Rational(1, 2), std::abs(Rational(1, -2)));
   ASSERT_EQ(Rational(0, 2), std::abs(Rational(0, -3)));
   ASSERT_EQ(Rational(0, 2), std::abs(Rational(0, 3)));
}