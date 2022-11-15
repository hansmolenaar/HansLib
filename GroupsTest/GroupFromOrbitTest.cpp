#include <gtest/gtest.h>

#include "Groups/GroupFromOrbit.h"
#include "Geometry/PointTransformationIdentity.h"
#include "Geometry/PointClose.h"
#include "Geometry/Rotate2D.h"

#include <numbers>

TEST(GroupFromOrbitTest, Trivial)
{
   const  PointTransformationIdentity<double, 1> transform;
   const  std::vector<const IPointTransformation<double, 1>*> transformations{ &transform };
   const PointClose<double, 1> areClose;
   const auto retval = GroupFromOrbit::Create(areClose, transformations, Point1{ 3.0 });

   ASSERT_EQ(retval->getOrder(), 1);
   ASSERT_EQ(retval->getIdentity(), 0);
}


TEST(GroupFromOrbitTest, Rotate4)
{
   const  Rotate2D rotate0(0);
   const  Rotate2D rotate1(std::numbers::pi/2);
   const  Rotate2D rotate2(std::numbers::pi );
   const  Rotate2D rotate3(3*std::numbers::pi/2);
   const  std::vector<const IPointTransformation<double, 2>*> transformations{ &rotate0, &rotate1, &rotate2, &rotate3 };
   const PointClose<double, 2> areClose;
   const auto retval = GroupFromOrbit::Create(areClose, transformations, Point2{ 1, 0 });

   ASSERT_EQ(retval->getOrder(), 4);
   ASSERT_EQ(retval->getIdentity(), 0);
   ASSERT_TRUE(false); // TODO more testing
}

