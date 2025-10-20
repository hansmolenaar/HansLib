#include <gtest/gtest.h>

#include "HatFunction.h"
#include "ISingleVariableRealValuedFunctionUtils.h"

inline constexpr double Epsilon = 1.0e-12;

TEST(HatFunctionTest, Test0)
{
    const HatFunction fie;
    ASSERT_TRUE(fie.IsNonConstant());
    ASSERT_FALSE(fie.HasDerivative());
    ASSERT_NEAR(fie.Evaluate(-2.0), 0.0, Epsilon);
    ASSERT_NEAR(fie.Evaluate(-1.0), 0.0, Epsilon);
    ASSERT_NEAR(fie.Evaluate(-0.5), 0.5, Epsilon);
    ASSERT_NEAR(fie.Evaluate(0.0), 1.0, Epsilon);
    ASSERT_NEAR(fie.Evaluate(0.5), 0.5, Epsilon);
    ASSERT_NEAR(fie.Evaluate(1.0), 0.0, Epsilon);
    ASSERT_NEAR(fie.Evaluate(2.0), 0.0, Epsilon);
    ASSERT_EQ(fie.Evaluate(0.1), fie(0.1));

    // ISingleVariableRealValuedFunctionUtils::ToFile(fie, -2.0, 2.0, 1000, "HatFunction");
}
