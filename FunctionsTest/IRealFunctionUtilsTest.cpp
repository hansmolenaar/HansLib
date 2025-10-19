#include <gtest/gtest.h>

#include "RealFunctionAdd.h"
#include "MultiVariableMonomial.h"
#include "IRealFunctionUtils.h"
#include "IMultiVariableRealValuedFunction.h"
#include "MultiVariableRealValuedFunction.h"
#include "MultiVariableVectorValuedFunction.h"
#include "MatrixDense.h"
#include "RealFunctionCheckDerivative.h"

#include <array>
#include <memory>

namespace {
	const double c_eps = 1.0e-12;
}



TEST(IRealFunctionUtilsTest, TestAdd)
{
	std::shared_ptr<IMultiVariableRealValuedFunction> fie00(new MultiVariableMonomial({ 0, 0, 3 }));
	auto fie0 = MultiVariableVectorValuedFunction(std::span<std::shared_ptr<IMultiVariableRealValuedFunction>>(&fie00, 1));

	std::shared_ptr<IMultiVariableRealValuedFunction> fie11(new MultiVariableMonomial({ 2, 0, 0 }));
	auto fie1 = MultiVariableVectorValuedFunction(std::span<std::shared_ptr<IMultiVariableRealValuedFunction>>(&fie11, 1));

	auto fie_ptr = fie0 + fie1;
	const IRealFunction& fie = *fie_ptr;

	ASSERT_EQ(3, fie.GetDomainDimension());
	ASSERT_EQ(1, fie.GetRangeDimension());

	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 0));
	ASSERT_TRUE(fie.DerivativeAlwaysZero(0, 1));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 2));



	double x[] = { 1, 2, 3 };
	double y[1];
	fie.Evaluate(x, y);
	ASSERT_NEAR(y[0], 28.0, c_eps);

	MatrixDense deriv(1, 3);
	fie.Derivative(x, deriv);

	ASSERT_EQ(deriv(0, 0), 2.0);
	ASSERT_EQ(deriv(0, 1), 0.0);
	ASSERT_EQ(deriv(0, 2), 27.0);

	double delx[] = { 0.01, 0.01, 0.01 };
	RealFunctionCheckDerivative::Check(fie, x, delx);
}







