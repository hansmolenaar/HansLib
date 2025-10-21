#include <gtest/gtest.h>

#include "CyclicGroup.h"
#include "DihedralGroup.h"
#include "IFiniteGroupUtils.h"

TEST(IFiniteGroupUtilsTest, IsAbelian_cyclic)
{
    for (int groupSize = 0; groupSize < 5; ++groupSize)
    {
        const auto group = CyclicGroup::Create(groupSize);
        ASSERT_TRUE(IFiniteGroupUtils::IsAbelian(*group));
    }
}

TEST(IFiniteGroupUtilsTest, IsAbelian_dihedral)
{
    ASSERT_TRUE(IFiniteGroupUtils::IsAbelian(*DihedralGroup::Create(0)));
    ASSERT_TRUE(IFiniteGroupUtils::IsAbelian(*DihedralGroup::Create(1)));
    ASSERT_TRUE(IFiniteGroupUtils::IsAbelian(*DihedralGroup::Create(2)));
    ASSERT_FALSE(IFiniteGroupUtils::IsAbelian(*DihedralGroup::Create(3)));
    ASSERT_FALSE(IFiniteGroupUtils::IsAbelian(*DihedralGroup::Create(4)));
}