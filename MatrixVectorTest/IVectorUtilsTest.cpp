#include <gtest/gtest.h>


#include "MatrixVector/IVectorUtils.h"
#include "MatrixVector/VectorWrapper.h"

static const double eps = 1.0e-10;

TEST(IVectorUtilsTest, TestMult)
{
	std::vector<double> vals = {2,3};
	VectorWrapper vec(vals);
	vec *= 2.0;

	ASSERT_EQ(vec(0), 4.0);
	ASSERT_EQ(vec(1), 6.0);
}


