#include <gtest/gtest.h>

#include "IdentityMatrix.h"
#include "IMatrixTest.h"


TEST(IdentityMatrixTest, Test1)
{
	const IdentityMatrix mat(1);
	ASSERT_EQ(mat(0, 0), 1.0);
	IMatrixTest::TestConstInterface(mat);
}


TEST(IdentityMatrixTest, Test2)
{
	const IdentityMatrix mat(2);
	ASSERT_EQ(mat(0, 0), 1.0);
	ASSERT_EQ(mat(1, 1), 1.0);
	ASSERT_EQ(mat(1, 0), 0.0);
	ASSERT_EQ(mat(0, 1), 0.0);
	IMatrixTest::TestConstInterface(mat);
}


TEST(IdentityMatrixTest, TestError)
{
	ASSERT_ANY_THROW(IdentityMatrix(0); );
	ASSERT_ANY_THROW(IdentityMatrix(-1); );
	IdentityMatrix mat(1);
	ASSERT_ANY_THROW(mat(0, 0) = 2;);

	const IdentityMatrix cmat(2);
	IMatrixTest::TestConstInterface(mat);
}


TEST(IdentityMatrixTest, TestInstance)
{
	const IdentityMatrix & mat2 = IdentityMatrix::GetInstance(2);
	ASSERT_EQ(2, mat2.GetRowDimension());
	ASSERT_EQ(2, mat2.GetColDimension());

	const IdentityMatrix & anotherMat2 = IdentityMatrix::GetInstance(2);
	ASSERT_EQ(&mat2, &anotherMat2);
}






