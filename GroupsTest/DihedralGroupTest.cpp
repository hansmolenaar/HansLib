#include <gtest/gtest.h>

#include "Groups/DihedralGroup.h"
#include "Groups/IFiniteGroupUtils.h"

TEST(DihedralGroupTest, Basic1)
{
   for (int groupSize = 1; groupSize < 7; ++groupSize)
   {
      const auto group = DihedralGroup::Create(groupSize);
      IFiniteGroupUtils::CheckGroupAxioms(*group);
   }
}


TEST(DihedralGroupTest, KleinGroup)
{
   const auto kleinGroupPtr = DihedralGroup::Create(2);
   const auto& kleinGroup = *kleinGroupPtr;

   ASSERT_EQ(kleinGroup(0, 0), 0);
   ASSERT_EQ(kleinGroup(1, 1), 0);
   ASSERT_EQ(kleinGroup(2, 2), 0);
   ASSERT_EQ(kleinGroup(3, 3), 0);

   ASSERT_EQ(kleinGroup(0, 1), 1);
   ASSERT_EQ(kleinGroup(0, 2), 2);
   ASSERT_EQ(kleinGroup(0, 3), 3);
   ASSERT_EQ(kleinGroup(1, 0), 1);
   ASSERT_EQ(kleinGroup(2, 0), 2);
   ASSERT_EQ(kleinGroup(3, 0), 3);

   ASSERT_EQ(kleinGroup(1, 2), 3);
   ASSERT_EQ(kleinGroup(2, 1), 3);

   ASSERT_EQ(kleinGroup(1, 3), 2);
   ASSERT_EQ(kleinGroup(2, 3), 1);
   ASSERT_EQ(kleinGroup(3, 1), 2);
   ASSERT_EQ(kleinGroup(3, 2), 1);
}


TEST(DihedralGroupTest, TriangleIsNotAbelian)
{
   const auto d3 = DihedralGroup::Create(3);
   const auto& g = *d3;

   ASSERT_EQ(g(2, 2), 1);
   ASSERT_EQ(g(5, 5), 0);
   ASSERT_EQ(g(1, 5), 4);
   ASSERT_EQ(g(5, 1), 3);
}
