#include <gtest/gtest.h>

#include "IFinitePointGroupTest.h"

#include "Groups/FinitePointGroupRotation2.h"
#include "Groups/IGroupTable.h"

#include <array>


TEST(FinitePointGroupRotation2Test, TestInstance)
{
   const int order = 2;
   const auto& g2 = FinitePointGroupRotation2::GetInstance(order);
	ASSERT_EQ(g2.GroupTable().Order(),order);

   const auto& g2_another = FinitePointGroupRotation2::GetInstance(order);
   ASSERT_EQ(&g2, &g2_another);
}



TEST(FinitePointGroupRotation2Test, TestGroupTable)
{
   for (int order = 1; order < 10; ++order)
   {
      const auto& group = FinitePointGroupRotation2::GetInstance(order);
      std::array<double, 2> point = {1,2};
      IFinitePointGroupTest::TestPointGroup(group, point);
   }
}





