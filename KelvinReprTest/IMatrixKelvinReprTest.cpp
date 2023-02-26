#include <gtest/gtest.h>

#include "IMatrixKelvinReprTest.h"

//#include "IMatrixTest.h"
#include "IMatrixKelvinRepr.h"
#include "IMatrixKelvinReprUtils.h"

#include <numeric>
#include <ranges>

namespace IMatrixKelvinReprTest
{

	void TestInterface(IMatrixKelvinRepr& matrix)
	{
		const double eps = 1.0e-10;
	//	IMatrixTest::TestConstInterface(matrix);
		const int dim = matrix.GetDimension();
		ASSERT_GT(dim, 0);
		ASSERT_EQ(dim, matrix.GetColDimension());
		ASSERT_EQ(dim, matrix.GetRowDimension());

		ASSERT_ANY_THROW(matrix(0, 0) = 0;);

		ASSERT_ANY_THROW(matrix.Set(-1, 0, 0.0));
		ASSERT_ANY_THROW(matrix.Set(0, -1, 0.0));
		ASSERT_ANY_THROW(matrix.Set(dim, 0, 0.0));
		ASSERT_ANY_THROW(matrix.Set(0, dim, 0.0));

		for (int row = 0; row < dim; ++row)
		{
			for (int col = 0; col < dim; ++col)
			{
				Clear(matrix);
				matrix.Set(row, col, 1.0);
				ASSERT_NEAR(matrix.Get(row, col), 1.0, eps);
				ASSERT_NEAR(matrix.Get(col, row), 1.0, eps);
				ASSERT_NEAR(((const IMatrixKelvinRepr&)matrix)(row, col), 1.0, eps);
				ASSERT_NEAR(((const IMatrixKelvinRepr&)matrix)(col, row), 1.0, eps);
			}
		}

		SetAll(matrix, 1.0);
		double sum = 0;
		for (int row = 0; row < dim; ++row)
		{
			for (int col = 0; col < dim; ++col)
			{
				sum += matrix.Get(row, col)*matrix.Get(col, row);
			}
		}
		ASSERT_NEAR(sum, dim*dim, eps);

		const auto& vec = matrix.Vector();
		sum = std::inner_product(vec.begin(), vec.end(), vec.begin(), 0.0);
		ASSERT_NEAR(sum, dim*dim, eps);
	}
};








