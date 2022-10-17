#include <gtest/gtest.h>

#include "MatrixVector/IMatrixSquareUtils.h"
#include "MatrixVector/MatrixSquare.h"
#include "MatrixVector/IdentityMatrix.h"

#include <array>

namespace {
	constexpr double c_eps = 1.0e-12;
}

TEST(IMatrixSquareUtilsTest, Test1)
{
	MatrixSquare matrix(1);
	matrix(0, 0) = 3.0;
	ASSERT_EQ(GetDeterminant(matrix), 3.0);
}


TEST(IMatrixSquareUtilsTest, Test2)
{
	MatrixSquare matrix(2);
	matrix(0, 0) = 1;
	matrix(0, 1) = 2;
	matrix(1, 0) = 3;
	matrix(1, 1) = 4;
	ASSERT_NEAR(GetDeterminant(matrix), -2.0, c_eps);
}



TEST(IMatrixSquareUtilsTest, Test3)
{
	MatrixSquare matrix(3);
	matrix(0, 0) = 1;
	matrix(0, 1) = 2;
	matrix(0, 2) = 3;

	matrix(1, 0) = 4;
	matrix(1, 1) = 5;
	matrix(1, 2) = 6;

	matrix(2, 0) = 7;
	matrix(2, 1) = 8;
	matrix(2, 2) = 9;
	ASSERT_NEAR(GetDeterminant(matrix), 0.0, c_eps);
}

TEST(IMatrixSquareUtilsTest, Test4)
{
	MatrixSquare matrix(3);
	matrix(0, 0) = 1;
	matrix(0, 1) = 6;
	matrix(0, 2) = 4;

	matrix(1, 0) = 2;
	matrix(1, 1) = 7;
	matrix(1, 2) = 3;

	matrix(2, 0) = 8;
	matrix(2, 1) = 9;
	matrix(2, 2) = 5;
	ASSERT_NEAR(GetDeterminant(matrix), -60.0, c_eps);
}

#if false // TODO
TEST(IMatrixSquareUtilsTest, TestDeterminantDerivs2)
{
	MatrixSquare matrix(2);
	matrix(0, 0) = 1;
	matrix(0, 1) = 2;
	matrix(1, 0) = 3;
	matrix(1, 1) = 4;

	ASSERT_NEAR(-2.0, GetDeterminant(matrix), c_eps);

	ASSERT_NEAR(4.0, GetDeterminantDerivative(matrix, 0, 0), c_eps);
	ASSERT_NEAR(1.0, GetDeterminantDerivative(matrix, 1, 1), c_eps);
	ASSERT_NEAR(-3.0, GetDeterminantDerivative(matrix, 0, 1), c_eps);
	ASSERT_NEAR(-2.0, GetDeterminantDerivative(matrix, 1, 0), c_eps);

	const int dim = 2;
	std::array<double, dim> roots;
	std::array<double, dim> derivs;

	for (int row = 0; row < dim; ++row)
	{
		for (int col = 0; col < dim; ++col)
		{
			matrix(0, 0) = 1;
			matrix(0, 1) = 2;
			matrix(1, 0) = 3;
			matrix(1, 1) = 4;
			SingleVariableRealValuedFunction fie(
				[&](double x) {matrix(row,col)=x; return GetDeterminant(matrix); },
				[&](double x) {matrix(row, col) = x; return GetDeterminantDerivative(matrix, row, col); });
			CheckDerivative(fie, 0.5, 0.1, true);
		}
	}
}
#endif

TEST(IMatrixSquareUtilsTest, TestTrace)
{
	const int dim = 10;
	const IdentityMatrix matrix(dim);
	const double trace = GetTrace(matrix);
	ASSERT_NEAR(dim, trace, c_eps);
}



