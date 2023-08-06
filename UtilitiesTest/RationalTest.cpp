#include <gtest/gtest.h>


#include "Rational.h"


TEST(RationalTest, DoesThisCompile)
{
   const Rational r2(1, 2);
   const Rational r3(1, 3);
   ASSERT_EQ(r2 - r3, Rational(1, 6));
   ASSERT_EQ(r2 + r3, Rational(5, 6));
   const auto r23 = r2 + Rational(1,6);
   ASSERT_EQ(r23, Rational(2, 3));
}