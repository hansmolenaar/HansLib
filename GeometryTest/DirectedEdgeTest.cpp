#include <gtest/gtest.h>

#include "DirectedEdge.h"
#include "PointClose.h"


TEST(DirectedEdgeTest, UnhappyPath)
{
   const PointClose<double, 1> areClose;
   const std::array<double, 1> point{ 1.0 };
   ASSERT_TRUE(areClose.SamePoints(point, point));
   //auto edge = DirectedEdge<double, 1>::Create(point, std::array<double, 1>{2}, areClose);
   // TODO what is wrong here?
   //ASSERT_ANY_THROW(DirectedEdge<double, 1>::Create(point, point, areClose));
}
