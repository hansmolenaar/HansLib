#include <gtest/gtest.h>

#include "MultiVariableRealValuedFunctionNoDerivatives.h"
#include "MultiVariableMonomial.h"
#include "Functors.h"

#include <array>

TEST(MultiVariableRealValuedFunctionNoDerivativesTest, Test1)
{
   std::unique_ptr<IMultiVariableFunctionEvaluate> eval = std::make_unique<MultiVariableMonomial>(std::vector<int>{ 2, 1 });
   const MultiVariableRealValuedFunctionNoDerivatives fie(2, std::move(eval));

   ASSERT_EQ(fie.GetDomainDimension(), 2);
   ASSERT_EQ(fie.getDimension(), 2);
   ASSERT_EQ(fie.GetRangeDimension(), 1);

   const std::array<double, 2> xy{ 2,3 };
   std::vector<double> deriv;
   ASSERT_TRUE(Functors::AreClose()(fie(xy), 12.0));
   ASSERT_TRUE(Functors::AreClose()(fie.Evaluate(xy), 12.0));
   ASSERT_THROW(fie.Derivative(xy, deriv), MyException);
   ASSERT_THROW(fie.DerivativeAlwaysZero(0), MyException);
   ASSERT_FALSE(fie.HasDerivative());
}
