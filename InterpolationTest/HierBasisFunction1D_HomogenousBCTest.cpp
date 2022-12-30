

#include <gtest/gtest.h>

#include "Interpolation/HierBasisFunction1D_HomogenousBC.h"
#include "Functions/ISingleVariableRealValuedFunctionUtils.h"
#include "Utilities/MyException.h"
#include "IHierBasisFunction1DTest.h"

#include "Utilities/Defines.h"


inline constexpr double Epsilon = 1.0e-10;

TEST(HierBasisFunction1D_HomogenousBCTest, Basis_1_1)
{
   const HierBasisFunction1D_HomogenousBC_Factory factory;
   const auto lowestLevel = factory.getLowestLevel();
   ASSERT_EQ(lowestLevel.size(), 1);
   ASSERT_EQ(lowestLevel.front(), HierLevelIndex(1, 1));
   ASSERT_TRUE(factory.canBeRefined(lowestLevel.front()));
   const auto fiePtr = factory.create(lowestLevel.front());
   const auto& fie = *fiePtr;

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.25), 0.5, Epsilon);
   ASSERT_NEAR(fie(0.5), 1, Epsilon);
   ASSERT_NEAR(fie(0.75), 0.5, Epsilon);
   ASSERT_NEAR(fie(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);

   //ISingleVariableRealValuedFunctionUtils::ToFile(fie, -2.0, 2.0, 1000, "HierBasisFunction1D_HomogenousBC_1_1");
}


TEST(HierBasisFunction1D_HomogenousBCTest, Basis_3_7)
{
   const HierBasisFunction1D_HomogenousBC fie(HierLevelIndex{ 3,7 });
   ASSERT_EQ(fie.getLevelIndex(), HierLevelIndex(3, 7));

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.75), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.875), 1.0, Epsilon);
   ASSERT_NEAR(fie(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);

   //ISingleVariableRealValuedFunctionUtils::ToFile(fie, -2.0, 2.0, 1000, "HierBasisFunction1D_HomogenousBC_3_7");
}

TEST(HierBasisFunction1D_HomogenousBCTest, Basis_5_9)
{
   const HierBasisFunction1D_HomogenousBC fie(HierLevelIndex{ 5,9 });
   IHierBasisFunction1DTest_Interface(fie);
}
