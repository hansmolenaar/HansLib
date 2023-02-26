#include <gtest/gtest.h>


#include "MatrixSquare.h"
#include "IMatrixUtils.h"
#include "IdentityMatrix.h"
#include "SubMatrix.h"
#include "MatrixDense.h"

namespace {
   const double c_eps = 1.0e-12;
}

TEST(IMatrixUtilsTest, Test1)
{
	MatrixSquare matrix(1);
	matrix(0, 0) = 1.0;
	matrix *= 2.0;
	ASSERT_EQ(matrix(0, 0), 2.0);
}


TEST(IMatrixUtilsTest, Test2)
{
	IdentityMatrix imat(10);
	const SubMatrix mat(imat, 2, 3, 0, 0);
	ASSERT_ANY_THROW(CheckRowCol(mat, -1, 0));
	ASSERT_ANY_THROW(CheckRowCol(mat, 0, -1));
	ASSERT_ANY_THROW(CheckRowCol(mat, 2, 0));
	ASSERT_ANY_THROW(CheckRowCol(mat, 0, 3));
}



TEST(IMatrixUtilsTest, Test3)
{
	MatrixSquare matrix(1);
	matrix(0, 0) = 3.0;
	matrix += IdentityMatrix::GetInstance(1);
	ASSERT_NEAR(matrix(0, 0), 4.0, c_eps);
}


TEST(IMatrixUtilsTest, Test4)
{
	MatrixSquare matrix(2);
	ASSERT_ANY_THROW(matrix += IdentityMatrix::GetInstance(1));
}


TEST(IMatrixUtilsTest, TestMatrixTimesVector)
{
   MatrixDense matrix(1,2);
   matrix(0, 0) = 1;
   matrix(0, 1) = 2;

   std::vector<double> vecin(1, 3.0);
   std::vector<double> vecout;
   ASSERT_ANY_THROW(MatrixTimesVector(matrix,vecin, vecout));
   
   vecin.push_back(1.0);
   ASSERT_ANY_THROW(MatrixTimesVector(matrix, vecin, vecout));

   vecout.push_back(1.0);
   MatrixTimesVector(matrix, vecin, vecout);

   ASSERT_NEAR(vecout[0], 5, c_eps);
}



TEST(IMatrixUtilsTest, TestMatrixTimesMatrix)
{
   MatrixDense matrix1(1, 2);
   matrix1(0, 0) = 1;
   matrix1(0, 1) = 2;

   MatrixDense matrix2(2, 1);
   matrix2(0, 0) = 3;
   matrix2(1, 0) = 4;

   MatrixDense result(2, 1);
   ASSERT_ANY_THROW(MatrixTimesMatrix(matrix1, matrix2, result));
   ASSERT_ANY_THROW(MatrixTimesMatrix(matrix2, matrix1, result));

   result = MatrixDense(1, 1);
   ASSERT_ANY_THROW(MatrixTimesMatrix(matrix2, matrix1, result));

   MatrixTimesMatrix(matrix1, matrix2, result);
   ASSERT_NEAR(result(0,0), 11, c_eps);

   result = MatrixDense(2,2);
   ASSERT_ANY_THROW(MatrixTimesMatrix(matrix1, matrix2, result));

   MatrixTimesMatrix(matrix2, matrix1, result);
   ASSERT_NEAR(result(0, 0), 3, c_eps);
   ASSERT_NEAR(result(0, 1), 6, c_eps);
   ASSERT_NEAR(result(1, 0), 4, c_eps);
   ASSERT_NEAR(result(1, 1), 8, c_eps);
}
