#include <gtest/gtest.h>

#include "Groups/ActionReflect1D.h"
#include "IFinitePointGroupActionTest.h"

TEST(ActionReflect1DTest, Basic1)
{
   const ActionReflect1D groupAction;
   Test_IFinitePointGroupAction(groupAction, GetRandomPoint<1>());
}
