#include <gtest/gtest.h>

#include "RealFunctionAdd.h"
#include "MultiVariableMonomial.h"
#include "IRealFunctionUtils.h"
#include "IMultiVariableRealValuedFunction.h"
#include "MultiVariableVectorValuedFunction.h"
#include "RealFunctionCheckDerivative.h"
#include "MatrixDense.h"

#include <array>
#include <memory>

namespace {
	const double c_eps = 1.0e-12;
}

TEST(RealFunctionAddTest, Test1)
{
	auto fie1 = MultiVariableMonomial({ 0,1,2 });
	auto fie2 = MultiVariableMonomial({ 0,2,0 });

	auto fie = RealFunctionAdd(fie1, fie2);

	ASSERT_EQ(3, fie.GetDomainDimension());
	ASSERT_EQ(1, fie.GetRangeDimension());

	ASSERT_TRUE(fie.DerivativeAlwaysZero(0, 0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 1));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 2));

	std::array<double, 3> x = { 1, 2, 3 };
	ASSERT_NEAR(Evaluate(fie, x), 22.0, c_eps);

	std::vector<double> deriv(3);
	Derivative(fie, x, deriv);
	ASSERT_EQ(deriv[0], 0.0);
	ASSERT_EQ(deriv[1], 13.0);
	ASSERT_EQ(deriv[2], 12.0);

	std::array<double, 3> delx = { 0.0, 0.01, 0.01 };
	RealFunctionCheckDerivative::Check(fie, x, delx);
}


TEST(RealFunctionAddTest, Test2)
{
	std::shared_ptr<IMultiVariableRealValuedFunction> fie00(new MultiVariableMonomial({ 0, 0, 2 }));
	std::shared_ptr<IMultiVariableRealValuedFunction> fie01(new MultiVariableMonomial({ 1, 1, 0 }));
	std::array<std::shared_ptr<IMultiVariableRealValuedFunction>, 2> fies0 = { fie00, fie01 };
	auto fie0 = MultiVariableVectorValuedFunction(fies0);

	std::shared_ptr<IMultiVariableRealValuedFunction> fie10(new MultiVariableMonomial({ 0, 1, 0 }));
	std::shared_ptr<IMultiVariableRealValuedFunction> fie11(new MultiVariableMonomial({ 2, 0, 0 }));
	std::array<std::shared_ptr<IMultiVariableRealValuedFunction>, 2> fies1 = { fie10, fie11 };
	auto fie1 = MultiVariableVectorValuedFunction(fies1);

	RealFunctionAdd fie(fie0, fie1);

	ASSERT_EQ(3, fie.GetDomainDimension());
	ASSERT_EQ(2, fie.GetRangeDimension());

	ASSERT_TRUE(fie.DerivativeAlwaysZero(0, 0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 1));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 2));

	ASSERT_FALSE(fie.DerivativeAlwaysZero(1, 0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(1, 1));
	ASSERT_TRUE(fie.DerivativeAlwaysZero(1, 2));


	double x[] = { 1, 2, 3 };
	double y[2];
	fie.Evaluate(x, y);
	ASSERT_NEAR(y[0], 11.0, c_eps);
	ASSERT_NEAR(y[1], 3.0, c_eps);

	MatrixDense deriv(2, 3);
	fie.Derivative(x, deriv);

	ASSERT_EQ(deriv(0, 0), 0.0);
	ASSERT_EQ(deriv(0, 1), 1.0);
	ASSERT_EQ(deriv(0, 2), 6.0);

	ASSERT_EQ(deriv(1, 0), 4.0);
	ASSERT_EQ(deriv(1, 1), 1.0);
	ASSERT_EQ(deriv(1, 2), 0.0);

	double delx[] = { 0.01, 0.01, 0.01 };
	RealFunctionCheckDerivative::Check(fie, x, delx);
}







