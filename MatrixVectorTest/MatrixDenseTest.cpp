#include <gtest/gtest.h>

#include "MatrixDense.h"
#include "IMatrixTest.h"


TEST(MatrixDenseTest, TestConstructor)
{
	MatrixDense mat(2,3);
	ASSERT_EQ(2, mat.GetRowDimension());
	ASSERT_EQ(3, mat.GetColDimension());
}


TEST(MatrixDenseTest, TestInterface)
{
	MatrixDense mat(2, 3);
	IMatrixTest::TestConstInterface(mat);
}



TEST(MatrixDenseTest, TestGetSet)
{
	const int nRow = 5;
	const int nCol = 7;
	MatrixDense mat(nRow, nCol);
	double val = 0.0;
	for (int row = 0; row < nRow; ++row)
	{
		for (int col = 0; col < nCol; ++col)
		{
			val += 1.0;
			mat(row, col) = val;
		}
	}

	val = 0.0;
	for (int row = 0; row < nRow; ++row)
	{
		for (int col = 0; col < nCol; ++col)
		{
			val += 1.0;
			ASSERT_EQ(val, mat(row, col));
		}
	}
}







