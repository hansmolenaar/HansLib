#include <gtest/gtest.h>

#include "Interpolation/HierBasisFunction.h"
#include "Interpolation/HierBasisFunction1D_ExtendedLevelOneBC.h"
#include "Interpolation/HierBasisFunction1D_HomogenousBC.h"
#include "Utilities/MyException.h"
#include "Utilities/Defines.h"
#include "Utilities/Single.h"
#include "Utilities/Plotting.h"

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
   const HierBasisFunction basisFie(std::vector<const IHierBasisFunction1D*>{&basisFie_2_1, & basisFie_2_3});

   ASSERT_EQ(basisFie.GetDomainDimension(), 2);
   ASSERT_EQ(basisFie.getMultiIndex().getSupNormLevel(), 2);
   ASSERT_EQ(basisFie.getMultiIndex().getL1NormLevel(), 3);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.5, 0.75}), 1.0, Epsilon);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.25, 0.875}), 0.25, Epsilon);
   ASSERT_NEAR(basisFie(std::vector<double>{ 0.75, 0.75}), 0.5, Epsilon);

   ASSERT_TRUE(basisFie.contains(std::vector<double>{0.1, 0.9}));
   ASSERT_FALSE(basisFie.contains(std::vector<double>{0.9, 0.1}));
}


TEST(HierBasisFunctionTest, Factory)
{
   const HierBasisFunction1D_ExtendedLevelOneBC_Factory factory1D;
   const HierBasisFunction_Factory factory(2, &factory1D);
   const auto lowest = factory.getLowestLevel();
   ASSERT_EQ(lowest.size(), 9);
   std::vector<HierMultiIndex> basis_1_1;
   str::copy_if(lowest, std::back_inserter(basis_1_1), [&factory](const HierMultiIndex& hmi) {return factory.canBeRefined(hmi); });
   const auto* basisFunction = factory.get(Utilities::Single(basis_1_1));
   ASSERT_NEAR((*basisFunction)(std::array<double, 2>{0.5, 0.5}), 1.0, Epsilon);
   ASSERT_NEAR((*basisFunction)(std::array<double, 2>{0.75, 0.25}), 0.25, Epsilon);
}


TEST(HierBasisFunctionTest, Factory_plot)
{
   const HierBasisFunction1D_HomogenousBC_Factory factory1D;
   const HierBasisFunction_Factory factory(2, &factory1D);
   const auto* fiePtr = factory.get(HierMultiIndex(std::vector<HierLevelIndex>{ {2,1}, {2,3} }));
   ASSERT_NEAR((*fiePtr)(std::array<double, 2>{0.25, 0.75}), 1.0, Epsilon);
   const auto fie = [&fiePtr](const std::vector<double>& x) {return (*fiePtr)(x); };
   Plotting::MdFunctionsOnUnityToFile("Testje", 2, { fie }, 100);
}

