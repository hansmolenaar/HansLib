#include <gtest/gtest.h>


#include "MatrixKelvinRepr3.h"
#include "IndexerSymmetric.h"
#include "IMatrixSquareUtils.h"
#include "IMatrixKelvinReprTest.h"

TEST(MatrixKelvinRepr3Test, Test1)
{
	auto matrix = MatrixKelvinRepr3::CreateEmpty();
	ASSERT_EQ(3, matrix.GetDimension());
	ASSERT_EQ(3, matrix.GetRowDimension());
	ASSERT_EQ(3, matrix.GetColDimension());
	for (int n = 0; n < 6; ++n)
	{
		ASSERT_EQ(0.0, matrix.Vector()[n]);
	}
}


TEST(MatrixKelvinRepr3Test, TestInterface)
{
	auto matrix = MatrixKelvinRepr3::CreateEmpty();
	IMatrixKelvinReprTest::TestInterface(matrix);
}

TEST(MatrixKelvinRepr3Test, Test2)
{
	auto matrix = MatrixKelvinRepr3::CreateEmpty();
	IndexerSymmetric indexer(3);
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			matrix.Set(row, col, indexer.ToFlat(row, col));
		}
	}

	ASSERT_NEAR(GetDeterminant(matrix), 1.0, 1.0e-12);
}



