#include <gtest/gtest.h>

#include "CyclicGroup.h"
#include "IFiniteGroupUtils.h"

TEST(GroupTest, Basic1)
{
    for (int groupSize = 1; groupSize < 8; ++groupSize)
    {
        const auto group = CyclicGroup::Create(groupSize);
        IFiniteGroupUtils::CheckGroupAxioms(*group);
    }
}
