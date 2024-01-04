#include <gtest/gtest.h>

#include "Polygon2D.h"
#include "Rational.h"

TEST(Polygon2DTest, SignedArea2D)
{
   const Point<Rational, 2> p0(Rational{ 1,1 }, Rational{ 2,1 });
   const Point<Rational, 2> p1(Rational{ 3,1 }, Rational{ 2,1 });
   const Point<Rational, 2> p2(Rational{ 2,1 }, Rational{ 3,1 });
   const std::vector< Point<Rational, 2>> trianglePos{ p0, p1, p2 };
   auto area = Polygon2D::AreaSigned<Rational>(trianglePos);
   ASSERT_EQ(Rational(1, 1), area);

   const std::vector< Point<Rational, 2>> triangleNeg{ p1, p0, p2 };
   area = Polygon2D::AreaSigned<Rational>(triangleNeg);
   ASSERT_EQ(Rational(-1, 1), area);
}
