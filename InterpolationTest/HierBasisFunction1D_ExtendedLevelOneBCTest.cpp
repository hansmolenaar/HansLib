#include <gtest/gtest.h>

#include "Interpolation/HierBasisFunction1D_ExtendedLevelOneBC.h"
#include "ISingleVariableRealValuedFunctionUtils.h"
#include "MyException.h"
#include "IHierBasisFunction1DTest.h"

#include "Defines.h"


inline constexpr double Epsilon = 1.0e-10;

TEST(HierBasisFunction1D_ExtendedLevelOneBCTest, Basis_0_0)
{
   HierBasisFunction1D_ExtendedLevelOneBC_Factory factory;
   const auto lowestLevel = factory.getLowestLevel();
   ASSERT_EQ(lowestLevel.size(), 3);
   ASSERT_EQ(lowestLevel.front(), HierLevelIndex(0, 0));
   ASSERT_FALSE(factory.canBeRefined(lowestLevel.front()));
   const auto& fie = *factory.get(lowestLevel.front());

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.0), 1.0, Epsilon);
   ASSERT_NEAR(fie(0.25), 0.75, Epsilon);
   ASSERT_NEAR(fie(0.5), 0.5, Epsilon);
   ASSERT_NEAR(fie(0.75), 0.25, Epsilon);
   ASSERT_NEAR(fie(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);
}


TEST(HierBasisFunction1D_ExtendedLevelOneBCTest, Basis_0_1)
{
   HierBasisFunction1D_ExtendedLevelOneBC_Factory factory;
   const auto lowestLevel = factory.getLowestLevel();
   ASSERT_EQ(lowestLevel.at(1), HierLevelIndex(0, 1));
   ASSERT_FALSE(factory.canBeRefined(lowestLevel.at(1)));
   const auto& fie = *factory.get(lowestLevel.at(1));

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.25), 0.25, Epsilon);
   ASSERT_NEAR(fie(0.5), 0.5, Epsilon);
   ASSERT_NEAR(fie(0.75), 0.75, Epsilon);
   ASSERT_NEAR(fie(1.0), 1.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);
}


TEST(HierBasisFunction1D_ExtendedLevelOneBCTest, Basis_1_1)
{
   HierBasisFunction1D_ExtendedLevelOneBC_Factory factory;
   const auto lowestLevel = factory.getLowestLevel();
   ASSERT_EQ(lowestLevel.at(2), HierLevelIndex(1, 1));
   ASSERT_TRUE(factory.canBeRefined(lowestLevel.at(2)));
   const auto& fie = *factory.get(lowestLevel.at(2));

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.25), 0.5, Epsilon);
   ASSERT_NEAR(fie(0.5), 1.0, Epsilon);
   ASSERT_NEAR(fie(0.75), 0.5, Epsilon);
   ASSERT_NEAR(fie(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);
}

