#include <gtest/gtest.h>

#include "Interpolation/HierBasisFunction.h"
#include "Interpolation/HierBasisFunction1D_HomogenousBC.h"
#include "Utilities/MyException.h"


inline constexpr double Epsilon = 1.0e-10;

TEST(HierBasisFunctionTest, BasisFie2D)
{
   const HierBasisFunction1D_HomogenousBC basisFie1D(HierLevelIndex{ 1,1 });
   const HierBasisFunction basisFie(std::vector<const IHierBasisFunction1D*>{&basisFie1D, & basisFie1D});

   ASSERT_EQ(basisFie.GetDomainDimension(), 2);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.5, 0.5}), 1.0, Epsilon);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.25, 0.5}), 0.5, Epsilon);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.5, 0.25}), 0.5, Epsilon);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.75, 0.5}), 0.5, Epsilon);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.5, 0.75}), 0.5, Epsilon);
}


TEST(HierBasisFunctionTest, BasisFie2_1_3)
{
   const HierBasisFunction1D_HomogenousBC basisFie_2_1(HierLevelIndex{ 1,1 });
   const HierBasisFunction1D_HomogenousBC basisFie_2_3(HierLevelIndex{ 2,3 });
   const HierBasisFunction basisFie(std::vector<const IHierBasisFunction1D*>{&basisFie_2_1, &basisFie_2_3});

   ASSERT_EQ(basisFie.GetDomainDimension(), 2);
   ASSERT_EQ(basisFie.getMultiIndex().getSupNormLevel(), 2);
   ASSERT_EQ(basisFie.getMultiIndex().getL1NormLevel(), 3);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.5, 0.75}), 1.0, Epsilon);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.25, 0.875}), 0.25, Epsilon);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.75, 0.75}), 0.5, Epsilon);

   ASSERT_TRUE(basisFie.contains(std::vector<double>{0.1, 0.9}));
   ASSERT_FALSE(basisFie.contains(std::vector<double>{0.9, 0.1}));
}