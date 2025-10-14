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

TEST(RationalTest, Abs)
{
   ASSERT_EQ(Rational(1, 2), std::abs(Rational(1, 2)));
   ASSERT_EQ(Rational(1, 2), std::abs(Rational(-1, 2)));
   ASSERT_EQ(Rational(1, 2), std::abs(Rational(1, -2)));
   ASSERT_EQ(Rational(0, 2), std::abs(Rational(0, -3)));
   ASSERT_EQ(Rational(0, 2), std::abs(Rational(0, 3)));
}


TEST(RationalTest, TimesDouble)
{
   ASSERT_NEAR(Rational(1, 2) * 4.0, 2.0, 1.0e-12);
   ASSERT_NEAR(4.0 * Rational(1, 2), 2.0, 1.0e-12);
}

TEST(RationalTest, StreamInsertion0)
{
   std::ostringstream os;
   os << Rational(0, 2);
   ASSERT_EQ(os.str(), "0");
}

TEST(RationalTest, StreamInsertion1)
{
   std::ostringstream os;
   os << Rational(2, 2);
   ASSERT_EQ(os.str(), "1");
}

TEST(RationalTest, StreamInsertion2)
{
   std::ostringstream os;
   os << Rational(1, -7);
   ASSERT_EQ(os.str(), "-1/7");
}

TEST(RationalTest, StreamInsertion3)
{
   std::ostringstream os;
   os << Rational(-9, 3);
   ASSERT_EQ(os.str(), "-3");
}