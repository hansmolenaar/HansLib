#include <gtest/gtest.h>

#include "Groups/SameFiniteGroupPredicate.h"
#include "Groups/CyclicGroup.h"
#include "Groups/DihedralGroup.h"

TEST(SameFiniteGroupPredicateTest, Trivial)
{
   const auto group0  = CyclicGroup::Create(2);
   const auto group1 = DihedralGroup::Create(1);
   const SameFiniteGroupPredicate areEqual(*group0);
   ASSERT_TRUE(areEqual(*group1));
}


TEST(SameFiniteGroupPredicateTest, Di2)
{
   const auto group0 = CyclicGroup::Create(4);
   const auto group1 = DihedralGroup::Create(2);
   const SameFiniteGroupPredicate areEqual(*group0);
   ASSERT_FALSE(areEqual(*group1));
}

