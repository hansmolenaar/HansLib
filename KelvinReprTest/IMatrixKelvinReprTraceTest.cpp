#include <gtest/gtest.h>

#include "IMatrixKelvinReprTrace.h"
#include "IMatrixKelvinReprUtils.h"
#include "MatrixDense.h"
#include "MatrixKelvinRepr1.h"

namespace
{
constexpr double c_eps = 1.0e-12;
}

TEST(IMatrixKelvinReprTraceTest, Test1)
{
    auto trace = IMatrixKelvinReprTrace(1);
    ASSERT_EQ(1, trace.GetDomainDimension());
    ASSERT_EQ(1, trace.GetRangeDimension());
    ASSERT_FALSE(trace.DerivativeAlwaysZero(0, 0));

    double v[] = {2.0};
    double t = trace.Evaluate(v);
    ASSERT_NEAR(2.0, t, c_eps);

    MatrixDense jac(1, 1);
    trace.Derivative(v, jac);
    ASSERT_NEAR(1.0, jac(0, 0), c_eps);
}

TEST(IMatrixKelvinReprTraceTest, Test3)
{
    auto trace = IMatrixKelvinReprTrace(3);
    ASSERT_EQ(6, trace.GetDomainDimension());
    ASSERT_EQ(1, trace.GetRangeDimension());
    ASSERT_FALSE(trace.DerivativeAlwaysZero(0, 0));
    ASSERT_FALSE(trace.DerivativeAlwaysZero(0, 1));
    ASSERT_FALSE(trace.DerivativeAlwaysZero(0, 2));
    ASSERT_TRUE(trace.DerivativeAlwaysZero(0, 3));
    ASSERT_TRUE(trace.DerivativeAlwaysZero(0, 4));
    ASSERT_TRUE(trace.DerivativeAlwaysZero(0, 5));

    double v[] = {1, 2, 3, 4, 5, 6};
    double t = trace.Evaluate(v);
    ASSERT_NEAR(6.0, t, c_eps);

    MatrixDense jac(1, 6);
    trace.Derivative(v, jac);
    ASSERT_NEAR(1.0, jac(0, 0), c_eps);
    ASSERT_NEAR(1.0, jac(0, 1), c_eps);
    ASSERT_NEAR(1.0, jac(0, 2), c_eps);
    ASSERT_NEAR(0.0, jac(0, 3), c_eps);
    ASSERT_NEAR(0.0, jac(0, 4), c_eps);
    ASSERT_NEAR(0.0, jac(0, 5), c_eps);
}
