#include <gtest/gtest.h>


#include "KelvinRepr/MatrixKelvinRepr1.h"
#include "MatrixVector/IMatrixSquareUtils.h"
#include "IMatrixKelvinReprTest.h"

TEST(MatrixKelvinRepr1Test, TestInterface)
{
	MatrixKelvinRepr1 matrix;
	ASSERT_EQ(1, matrix.GetDimension());
	IMatrixKelvinReprTest::TestInterface(matrix);
}


TEST(MatrixKelvinRepr1Test, Test2)
{
	MatrixKelvinRepr1 matrix;
	matrix.Set(0, 0, 2.0);
	ASSERT_NEAR(GetDeterminant(matrix), 2.0, 1.0e-12);
}



