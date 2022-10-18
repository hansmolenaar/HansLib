#include <gtest/gtest.h>

#include "MatrixVector/IMatrix.h"
#include "MatrixVector/IMatrixUtils.h"
#include "MatrixVector/MatrixSquare.h"
#include "MatrixVector/MatrixDense.h"

namespace IMatrixTest
{

   TEST(IMatrixTest, TestCopyFrom)
   {
      const int dim = 3;
      MatrixSquare source(dim);
      MatrixDense target(dim, dim);
      for (int i = 0; i < dim; ++i)
      {
         for (int j = 0; j < dim; ++j)
         {
            source(i, j) = i * dim + j;
         }
      }
      target.CopyFrom(source);
      for (int i = 0; i < dim; ++i)
      {
         for (int j = 0; j < dim; ++j)
         {
            ASSERT_EQ(target(i, j), i * dim + j);
         }
      }
   }

   TEST(IMatrixTest, TestAssignmentError)
   {
      const int dim = 3;
      MatrixSquare source(dim);
      MatrixDense target(dim, dim + 1);
      ASSERT_ANY_THROW(target.CopyFrom(source););
      target = MatrixDense(dim + 1, dim);
      ASSERT_ANY_THROW(target.CopyFrom(source););
   }


   void TestConstInterface(const IMatrix& matrix)
   {
      const int rowDim = matrix.GetRowDimension();
      const int colDim = matrix.GetColDimension();

      ASSERT_GE(rowDim, 0);
      ASSERT_GE(colDim, 0);

      ASSERT_ANY_THROW(matrix(-1, 0););
      ASSERT_ANY_THROW(matrix(0, -1););
      ASSERT_ANY_THROW(matrix(rowDim, 0););
      ASSERT_ANY_THROW(matrix(0, colDim););
   }

   void TestInterface(IMatrix& matrix)
   {
      TestConstInterface(matrix);

      const int rowDim = matrix.GetRowDimension();
      const int colDim = matrix.GetColDimension();

      ASSERT_GT(rowDim, 0);
      ASSERT_GT(colDim, 0);

      ASSERT_ANY_THROW(matrix(-1, 0) = 0;);
      ASSERT_ANY_THROW(matrix(0, -1) = 0;);
      ASSERT_ANY_THROW(matrix(rowDim, 0) = 0;);
      ASSERT_ANY_THROW(matrix(0, colDim) = 0;);

      for (int row = 0; row < rowDim; ++row)
      {
         for (int col = 0; col < colDim; ++col)
         {
            Clear(matrix);
            matrix(row, col) = 1.0;
            ASSERT_EQ(matrix(row, col), 1.0);
         }
      }
   }
};








