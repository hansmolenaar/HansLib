#include <gtest/gtest.h>

#include "IFiniteGroupUtils.h"
#include "SymmetricGroup.h"

TEST(SymmetricGroupTest, Order0)
{
    const auto symgroup = SymmetricGroup::Create(0);
    ASSERT_EQ(0, symgroup->getOrder());
}

TEST(SymmetricGroupTest, Order3)
{
    const auto symgroup = SymmetricGroup::Create(3);
    ASSERT_EQ(6, symgroup->getOrder());

    const auto id = symgroup->getIdentity();
    ASSERT_EQ(id, symgroup->getInverse(id));
    ASSERT_EQ(id, (*symgroup)(id, id));

    const auto other = (id + 1) % 6;
    ASSERT_EQ(id, (*symgroup)(other, symgroup->getInverse(other)));
}
