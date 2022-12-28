

#include <gtest/gtest.h>

#include "Interpolation/HierarchicalBasisFunction.h"
#include "Utilities/MyException.h"

#include "Utilities/Defines.h"


inline constexpr double Epsilon = 1.0e-10;

TEST(HierarchicalBasisFunctionTest, Basics)
{
   const HierarchicalBasisFunction fie(HierarchicalMultiIndex(std::vector<HierarchicalLevelIndex>{HierarchicalLevelIndex{ 2,1 }, HierarchicalLevelIndex{ 1,1 }}));
   ASSERT_EQ(fie.getDimension(), 2);
   ASSERT_NEAR(fie.evaluate(std::vector<double>{0.125, 0.5}), 0.5, Epsilon);
   ASSERT_NEAR(fie.evaluate(std::vector<double>{0.25, 0.5}), 1.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(std::vector<double>{0.375, 0.5}), 0.5, Epsilon);
   ASSERT_NEAR(fie.evaluate(std::vector<double>{0.25, 0.25}), 0.5, Epsilon);
   ASSERT_NEAR(fie.evaluate(std::vector<double>{0.25, 0.75}), 0.5, Epsilon);
   ASSERT_NEAR(fie.evaluate(std::vector<double>{0.125, 0.75}), 0.25, Epsilon);
}
