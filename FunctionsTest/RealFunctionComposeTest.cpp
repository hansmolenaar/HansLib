#include <gtest/gtest.h>

#include "RealFunctionCompose.h"
#include "MultiVariableMonomial.h"
#include "IRealFunctionUtils.h"
#include "MultiVariableRealValuedFunction.h"
#include "MultiVariableVectorValuedFunction.h"
#include "RealFunctionNull.h"
#include "SingleVariableMonomial.h"
#include "SingleVariableRealValuedFunction.h"
#include "MatrixDense.h"

#include <array>
#include <memory>
#include <vector>

namespace {
	const double c_eps = 1.0e-12;
}

TEST(RealFunctionComposeTest, Test1)
{
	auto g = MultiVariableMonomial({ 0,2 });    // y^2

	// ( 0 )
	// ( x )
	// ( 0 )
	auto nul = MultiVariableRealValuedFunction::CreateNullShared(1);
	std::shared_ptr<IMultiVariableRealValuedFunction> ff(new MultiVariableMonomial({ 1 }));
	std::shared_ptr<IMultiVariableRealValuedFunction> fies[] = { nul, ff, nul };
	auto f = MultiVariableVectorValuedFunction(fies);
	auto fie = RealFunctionCompose(f, g);

	ASSERT_EQ(2, fie.GetDomainDimension());
	ASSERT_EQ(3, fie.GetRangeDimension());

	ASSERT_TRUE(fie.DerivativeAlwaysZero(0, 0));
	ASSERT_TRUE(fie.DerivativeAlwaysZero(0, 1));

	ASSERT_TRUE(fie.DerivativeAlwaysZero(1, 0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(1, 1));

	ASSERT_TRUE(fie.DerivativeAlwaysZero(2, 0));
	ASSERT_TRUE(fie.DerivativeAlwaysZero(2, 1));

	const double x[] = { 1, 2 };
	double y[3];
	fie.Evaluate(x, y);
	ASSERT_NEAR(y[0], 0.0, c_eps);
	ASSERT_NEAR(y[1], 4.0, c_eps);
	ASSERT_NEAR(y[2], 0.0, c_eps);

	MatrixDense jac(3, 2);
	fie.Derivative(x, jac);

	ASSERT_EQ(jac(0, 0), 0.0);
	ASSERT_EQ(jac(0, 1), 0.0);

	ASSERT_EQ(jac(1, 0), 0.0);
	ASSERT_EQ(jac(1, 1), 4.0);

	ASSERT_EQ(jac(2, 0), 0.0);
	ASSERT_EQ(jac(2, 1), 0.0);

	double delx[] = { 0.01, 0.01 };
	CheckDerivatives(fie, x, delx);
}


TEST(RealFunctionComposeTest, Test2)
{
	// g => 2x
	SingleVariableRealValuedFunction g([](double x) {return 2 * x; }, [](double x) {return 2; });

	// ( x^2 )
	// ( x^3 )
	std::shared_ptr<IMultiVariableRealValuedFunction> f2(new MultiVariableMonomial({ 2 }));
	std::shared_ptr<IMultiVariableRealValuedFunction> f3(new MultiVariableMonomial({ 3 }));
	std::shared_ptr<IMultiVariableRealValuedFunction> fies[] = { f2, f3 };
	auto f = MultiVariableVectorValuedFunction(fies);
	auto fie = RealFunctionCompose(f, g);

	ASSERT_EQ(1, fie.GetDomainDimension());
	ASSERT_EQ(2, fie.GetRangeDimension());

	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(1, 0));

	const double x[] = { 2 };
	double y[2];
	fie.Evaluate(x, y);
	ASSERT_NEAR(y[0], 16.0, c_eps);
	ASSERT_NEAR(y[1], 64.0, c_eps);

	MatrixDense jac(2, 1);
	fie.Derivative(x, jac);

	ASSERT_EQ(jac(0, 0), 16.0);
	ASSERT_EQ(jac(1, 0), 96.0);

	double delx[] = { 0.01 };
	CheckDerivatives(fie, x, delx);
}


TEST(RealFunctionComposeTest, Test3)
{

	// ( x^3 )
	// ( x^2 )
	std::shared_ptr<IMultiVariableRealValuedFunction> g0(new MultiVariableMonomial({ 0,2 }));
	std::shared_ptr<IMultiVariableRealValuedFunction> g1(new MultiVariableMonomial({ 3,0 }));
	std::shared_ptr<IMultiVariableRealValuedFunction> fies[] = { g0,g1 };
	auto g = MultiVariableVectorValuedFunction(fies);

	// f => 3x_0 + 2x_1
	MultiVariableRealValuedFunction f(
		std::vector<bool>(2, true),
		[](std::span<const double>x) {return 3 * x[0] + 2 * x[1]; },
		[](std::span<const double>x, std::span< double> df) {df[0] = 3; df[1] = 2; });

	auto fie = RealFunctionCompose(f, g);


	ASSERT_EQ(2, fie.GetDomainDimension());
	ASSERT_EQ(1, fie.GetRangeDimension());

	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 1));

	const double x[] = {1,2 };
	double y[1];
	fie.Evaluate(x, y);
	ASSERT_NEAR(y[0], 14.0, c_eps);


	MatrixDense jac(1, 2);
	fie.Derivative(x, jac);

	ASSERT_EQ(jac(0, 0), 6.0);
	ASSERT_EQ(jac(0, 1), 12.0);

	double delx[] = { 0.1, 0.1 };
	CheckDerivatives(fie, x, delx);
}








