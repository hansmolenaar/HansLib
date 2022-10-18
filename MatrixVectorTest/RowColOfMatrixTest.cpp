#include <gtest/gtest.h>

#include "MatrixVector/RowColOfMatrix.h"
#include "MatrixVector/MatrixSquare.h"

const double c_eps = 1.0e-10;

TEST(RowColOfMatrixTest, Test1)
{
	const int dim = 6;
	MatrixSquare mat(dim);
	for (int row = 0; row < dim; ++row)
	{
		for (int col = 0; col < dim; ++col)
		{
			mat(row, col) = row * 0.1 + col * 0.01;
		}
	}

	RowColOfMatrix rowVector(mat, 2, true, 2, 3);
	ASSERT_NEAR(rowVector(0), 0.23, c_eps);
	ASSERT_NEAR(rowVector(1), 0.24, c_eps);
	ASSERT_ANY_THROW(rowVector(2));


	RowColOfMatrix colVector(mat, 2, false, 2, 3);
	ASSERT_NEAR(colVector(0), 0.23, c_eps);
	ASSERT_NEAR(colVector(1), 0.33, c_eps);
	ASSERT_ANY_THROW(colVector(2));
}

TEST(RowColOfMatrixTest, Test2)
{
	MatrixSquare mat(3);
	ASSERT_ANY_THROW(RowColOfMatrix(mat, 2, true, 0, 2));
	ASSERT_ANY_THROW(RowColOfMatrix(mat, 2, false, 2, 0));
	ASSERT_ANY_THROW(RowColOfMatrix(mat, 1, true, 0, 3));
	ASSERT_ANY_THROW(RowColOfMatrix(mat, 1, true, 3, 0));
	ASSERT_ANY_THROW(RowColOfMatrix(mat, 1, false, 0, 3));
	ASSERT_ANY_THROW(RowColOfMatrix(mat, 1, false, 3, 0));

	ASSERT_ANY_THROW(RowColOfMatrix(mat, 1, false, -1, 0));
	ASSERT_ANY_THROW(RowColOfMatrix(mat, 1, true, -1, 0));
	ASSERT_ANY_THROW(RowColOfMatrix(mat, 1, false, 0, -1));
	ASSERT_ANY_THROW(RowColOfMatrix(mat, 1, true, 0, -1));

	ASSERT_ANY_THROW(RowColOfMatrix(mat, -1, true, 1, 1));
	ASSERT_ANY_THROW(RowColOfMatrix(mat, -1, false, 1, 1));
}





