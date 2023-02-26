#include <gtest/gtest.h>

#include "ActionReflectOrigin.h"
#include "IFinitePointGroupActionTest.h"

TEST(ActionReflectOriginTest, Basic1)
{
   const ActionReflectOrigin<1> groupAction;
   Test_IFinitePointGroupAction(groupAction, GetRandomPoint<1>());
}


TEST(ActionReflectOriginTest, Reflect3D)
{
   const ActionReflectOrigin<3> groupAction;
   Test_IFinitePointGroupAction(groupAction, GetRandomPoint<3>());
}
