#include <gtest/gtest.h>

#include "MatrixDense.h"
#include "RealFunctionNull.h"

TEST(RealFunctionNullTest, Test1)
{
    const int numVar = 2;
    const int numEqn = 3;
    RealFunctionNull fie(numVar, numEqn);

    ASSERT_EQ(numVar, fie.GetDomainDimension());
    ASSERT_EQ(numEqn, fie.GetRangeDimension());

    ASSERT_TRUE(fie.DerivativeAlwaysZero(numEqn - 1, numVar - 1));
    double x[numVar] = {1, 2};
    double y[numEqn] = {1, 1, 1};
    fie.Evaluate(x, y);
    ASSERT_EQ(0.0, y[0]);
    ASSERT_EQ(0.0, y[1]);
    ASSERT_EQ(0.0, y[2]);

    MatrixDense jac(numEqn, numVar);
    jac(numEqn - 1, numVar - 1) = 1.0;
    fie.Derivative(x, jac);
    ASSERT_EQ(0.0, jac(numEqn - 1, numVar - 1));
}
