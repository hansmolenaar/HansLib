#include <gtest/gtest.h>

#include "AlternatingGroup.h"
#include "IFiniteGroupUtils.h"


TEST(AlternatingGroupTest, Order0)
{
   const auto symgroup = AlternatingGroup::Create(0);
   ASSERT_EQ(0, symgroup->getOrder());
}

TEST(AlternatingGroupTest, Order5)
{
   const auto symgroup = AlternatingGroup::Create(5);
   ASSERT_EQ(60, symgroup->getOrder());

   const auto id = symgroup->getIdentity();
   ASSERT_EQ(id, symgroup->getInverse(id));
   ASSERT_EQ(id, (*symgroup)(id, id));

   const auto other = (id + 1) % 60;
   ASSERT_EQ(id, (*symgroup)(other, symgroup->getInverse(other)));
} 