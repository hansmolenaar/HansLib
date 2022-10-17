#include <gtest/gtest.h>

#include "Groups/GroupTableCyclic.h"
#include "IGroupTableTest.h"



TEST(GroupTableCyclicTest, TestInstance)
{
   const int order = 2;
   const GroupTableCyclic& g2 = GroupTableCyclic::Instance(order);
	ASSERT_EQ(g2.Order(),order);

   const GroupTableCyclic& g2_another = GroupTableCyclic::Instance(order);
   ASSERT_EQ(&g2, &g2_another);
}

TEST(GroupTableCyclicTest, TestInterface)
{
   for (int n = 1; n < 10; ++n)
   {
      IGroupTableTest::TestInterface(GroupTableCyclic::Instance(n));
  }
}








