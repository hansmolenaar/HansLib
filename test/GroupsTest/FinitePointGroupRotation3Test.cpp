#include <gtest/gtest.h>

#include "IFinitePointGroupTest.h"

#include "Groups/FinitePointGroupRotation3.h"
#include "Groups/IGroupTable.h"
#include "MatrixVector/UnitVector.h"
#include "Utils/MathConstants.h"

TEST(FinitePointGroupRotation3Test, TestInstance)
{
   const int order = 2;
   const auto axis = UnitVector3::TryCreate(std::array<double, 3>{1, 2, 3});
   const FinitePointGroupRotation3 g2(order, *axis);
   ASSERT_EQ(g2.GroupTable().Order(), order);
}



TEST(FinitePointGroupRotation3Test, TestGroupTable)
{
   const auto axis = UnitVector3::TryCreate(std::array<double, 3>{1, 2, 3});
   for (int order = 1; order < 10; ++order)
   {
      const FinitePointGroupRotation3 group(order, *axis);
      std::array<double, 3> point = { MathConstants::PI, MathConstants::E, MathConstants::LN10 };
      IFinitePointGroupTest::TestPointGroup(group, point);
   }
}





