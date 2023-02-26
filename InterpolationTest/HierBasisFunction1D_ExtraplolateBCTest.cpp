#include <gtest/gtest.h>

#include "HierBasisFunction1D_ExtraplolateBC.h"
#include "ISingleVariableRealValuedFunctionUtils.h"
#include "MyException.h"
#include "Single.h"
#include "IHierBasisFunction1DTest.h"

#include "Defines.h"


inline constexpr double Epsilon = 1.0e-10;

TEST(HierBasisFunction1D_ExtraplolateBCTest, Basis_1_1)
{
   HierBasisFunction1D_ExtraplolateBC_Factory factory;
   const auto lowestLevel = factory.getLowestLevel();
   const auto li_1_1 = Utilities::Single(lowestLevel);
   ASSERT_EQ(li_1_1, HierLevelIndex(1, 1));
   ASSERT_TRUE(factory.canBeRefined(li_1_1));
   const auto& fie = *factory.get(lowestLevel.front());

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.0), 1.0, Epsilon);
   ASSERT_NEAR(fie(0.25), 1.0, Epsilon);
   ASSERT_NEAR(fie(0.5), 1.0, Epsilon);
   ASSERT_NEAR(fie(0.75), 1.0, Epsilon);
   ASSERT_NEAR(fie(1.0), 1.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);

   ASSERT_TRUE(fie.getSupport().contains(0.0));
   ASSERT_TRUE(fie.getSupport().contains(1.0));
}


TEST(HierBasisFunction1D_ExtraplolateBCTest, Basis_2_1)
{
   HierBasisFunction1D_ExtraplolateBC_Factory factory;
   const HierBasisFunction1D_ExtraplolateBC fie({ 2,1 });
   ASSERT_TRUE(factory.canBeRefined({ 2,1 }));

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.0), 2.0, Epsilon);
   ASSERT_NEAR(fie(0.25), 1.0, Epsilon);
   ASSERT_NEAR(fie(0.5), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.75), .0, Epsilon);
   ASSERT_NEAR(fie(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);

   ASSERT_TRUE(fie.getSupport().contains(0.0));
   ASSERT_TRUE(fie.getSupport().contains(0.5));
   ASSERT_FALSE(fie.getSupport().contains(1.0));
}


TEST(HierBasisFunction1D_ExtraplolateBCTest, Basis_2_3)
{
   HierBasisFunction1D_ExtraplolateBC_Factory factory;
   const HierBasisFunction1D_ExtraplolateBC fie({ 2,3 });
   ASSERT_TRUE(factory.canBeRefined({ 2,3 }));

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.25), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.5), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.75), 1.0, Epsilon);
   ASSERT_NEAR(fie(1.0), 2.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);

   ASSERT_FALSE(fie.getSupport().contains(0.0));
   ASSERT_TRUE(fie.getSupport().contains(0.5));
   ASSERT_TRUE(fie.getSupport().contains(1.0));
}


TEST(HierBasisFunction1D_ExtraplolateBCTest, Basis_3_1)
{
   HierBasisFunction1D_ExtraplolateBC_Factory factory;
   const HierBasisFunction1D_ExtraplolateBC fie({ 3,1 });
   ASSERT_TRUE(factory.canBeRefined({ 3,1 }));

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.0), 2.0, Epsilon);
   ASSERT_NEAR(fie(0.125), 1.0, Epsilon);
   ASSERT_NEAR(fie(0.25), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.75), 0.0, Epsilon);
   ASSERT_NEAR(fie(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);

   ASSERT_TRUE(fie.getSupport().contains(0.0));
   ASSERT_TRUE(fie.getSupport().contains(0.25));
   ASSERT_FALSE(fie.getSupport().contains(1.0));
}


TEST(HierBasisFunction1D_ExtraplolateBCTest, Basis_3_3)
{
   HierBasisFunction1D_ExtraplolateBC_Factory factory;
   const HierBasisFunction1D_ExtraplolateBC fie({ 3,3 });
   ASSERT_TRUE(factory.canBeRefined({ 3,3 }));

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.25), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.375), 1.0, Epsilon);
   ASSERT_NEAR(fie(0.5), 0.0, Epsilon);
   ASSERT_NEAR(fie(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);

   ASSERT_FALSE(fie.getSupport().contains(0.0));
   ASSERT_TRUE(fie.getSupport().contains(0.25));
   ASSERT_TRUE(fie.getSupport().contains(0.5));
   ASSERT_FALSE(fie.getSupport().contains(1.0));
}


TEST(HierBasisFunction1D_ExtraplolateBCTest, Basis_3_7)
{
   HierBasisFunction1D_ExtraplolateBC_Factory factory;
   const HierBasisFunction1D_ExtraplolateBC fie({ 3,7 });
   ASSERT_TRUE(factory.canBeRefined({ 3,7 }));

   IHierBasisFunction1DTest_Interface(fie);

   ASSERT_NEAR(fie(-1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.25), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.75), 0.0, Epsilon);
   ASSERT_NEAR(fie(0.875), 1.0, Epsilon);
   ASSERT_NEAR(fie(1.0), 2.0, Epsilon);
   ASSERT_NEAR(fie(2.0), 0.0, Epsilon);

   ASSERT_FALSE(fie.getSupport().contains(0.0));
   ASSERT_FALSE(fie.getSupport().contains(0.25));
   ASSERT_TRUE(fie.getSupport().contains(0.75));
   ASSERT_TRUE(fie.getSupport().contains(1.0));
}
