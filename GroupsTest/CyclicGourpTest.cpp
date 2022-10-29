#include <gtest/gtest.h>

#include "Groups/CyclicGroup.h"
#include "Groups/IFiniteGroupUtils.h"

TEST(GroupTest, Basic1)
{
   for (int groupSize = 1; groupSize < 8; ++groupSize)
   {
      const auto group = CyclicGroup::Create(groupSize);
      IFiniteGroupUtils::CheckGroupAxioms(*group);
 }
}
