#include <gtest/gtest.h>

#include "Interpolation/HierarchicalApproximation1D.h"
#include "Functions/SingleVariablePolynomial.h"
#include "Utilities/MyException.h"
#include "Functions/SingleVariableFunctionExamples.h"
#include "Functions/SingleVariableRealValuedFunction.h"
#include "Functions/SingleVariableMonomial.h"

inline constexpr double Epsilon = 1.0e-10;

TEST(HierarchicalApproximation1DTest, Basis_0)
{
   SingleVariablePolynomial fie;
   fie.Add(2.0, SingleVariableMonomial(0));
   fie.Add(3.0, SingleVariableMonomial(1));

   auto approx = HierarchicalApproximation1D::CreateToLevel(fie, 0);
   ASSERT_NEAR((*approx)(std::vector<double>{0.0}), 2.0, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{0.25}), 1.0, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{0.5}), 0.0, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{0.75}), 2.5, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{1.0}), 5.0, Epsilon);
}


TEST(HierarchicalApproximation1DTest, Basis_1)
{
   SingleVariablePolynomial fie;
   fie.Add(2.0, SingleVariableMonomial(0));
   fie.Add(3.0, SingleVariableMonomial(1));

   auto approx = HierarchicalApproximation1D::CreateToLevel(fie, 1);
   ASSERT_NEAR((*approx)(std::vector<double>{0.0}), 2.0, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{0.25}), 2.75, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{0.5}), 3.5, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{0.75}), 4.25, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{1.0}), 5.0, Epsilon);
}


TEST(HierarchicalApproximation1DTest, Basis_5)
{
   SingleVariablePolynomial fie;
   fie.Add(2.0, SingleVariableMonomial(0));
   fie.Add(3.0, SingleVariableMonomial(1));

   auto approx = HierarchicalApproximation1D::CreateToLevel(fie, 5);
   ASSERT_NEAR((*approx)(std::vector<double>{0.0}), 2.0, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{0.25}), 2.75, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{0.5}), 3.5, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{0.75}), 4.25, Epsilon);
   ASSERT_NEAR((*approx)(std::vector<double>{1.0}), 5.0, Epsilon);
}


TEST(HierarchicalApproximation1DTest, isleaf)
{
   const auto rungeFie = SingleVariableFunctionExamples::GetRungeFunction();
   auto approx = HierarchicalApproximation1D::CreateToLevel(*rungeFie, 5);

   ASSERT_FALSE(approx->isLeaf(HierarchicalLevelIndex(0, 0)));
   ASSERT_FALSE(approx->isLeaf(HierarchicalLevelIndex(0, 1)));
   ASSERT_FALSE(approx->isLeaf(HierarchicalLevelIndex(1, 1)));
   ASSERT_FALSE(approx->isLeaf(HierarchicalLevelIndex(4, 7)));
   ASSERT_TRUE(approx->isLeaf(HierarchicalLevelIndex(5, 7)));
}



TEST(HierarchicalApproximation1DTest, Square)
{
   const SingleVariableMonomial square(2);
   constexpr size_t maxLevel = 8;
   std::vector<double> maxSurplus;
   for (size_t n = 1; n < maxLevel; ++n)
   {
      const auto approx = HierarchicalApproximation1D::CreateToLevel(square, n);
      maxSurplus.push_back(approx->getMaxSurplus());
   }

   for (size_t n = 1; n < maxSurplus.size(); ++n)
   {
      ASSERT_TRUE(maxSurplus.at(n) < 0.5 * maxSurplus.at(n - 1));
   }
}


TEST(HierarchicalApproximation1DTest, NumLeaves)
{
   const SingleVariableMonomial square(2);
   ASSERT_EQ(HierarchicalApproximation1D::CreateToLevel(square, 0)->numLeaves(), 0);
   ASSERT_EQ(HierarchicalApproximation1D::CreateToLevel(square, 1)->numLeaves(), 1);
   ASSERT_EQ(HierarchicalApproximation1D::CreateToLevel(square, 2)->numLeaves(), 2);
   ASSERT_EQ(HierarchicalApproximation1D::CreateToLevel(square, 3)->numLeaves(), 4);
   ASSERT_EQ(HierarchicalApproximation1D::CreateToLevel(square, 4)->numLeaves(), 8);
}
