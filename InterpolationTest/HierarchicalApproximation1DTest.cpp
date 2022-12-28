#include <gtest/gtest.h>

#include "Interpolation/HierarchicalApproximation1D.h"
#include "Functions/SingleVariablePolynomial.h"
#include "Utilities/MyException.h"

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
