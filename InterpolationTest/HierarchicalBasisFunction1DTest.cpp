#include <gtest/gtest.h>

#include "Interpolation/HierarchicalBasisFunction1D.h"
#include "Utilities/MyException.h"

inline constexpr double Epsilon = 1.0e-10;

TEST(HierarchicalBasisFunction1D, Basis_0_0)
{
   const  HierarchicalBasisFunction1D fie(HierarchicalLevelIndex(0, 0));

   ASSERT_NEAR(fie.evaluate(-0.1), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.0), 1.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.25), 0.5, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.5), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.75), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.1), 0.0, Epsilon);
}


TEST(HierarchicalBasisFunction1D, Basis_0_1)
{
   const  HierarchicalBasisFunction1D fie(HierarchicalLevelIndex(0, 1));

   ASSERT_NEAR(fie.evaluate(-0.1), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.25), 0., Epsilon);
   ASSERT_NEAR(fie.evaluate(0.5), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.75), 0.5, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.0), 1.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.1), 0.0, Epsilon);
}


TEST(HierarchicalBasisFunction1D, Basis_1_1)
{
   const  HierarchicalBasisFunction1D fie(HierarchicalLevelIndex(1, 1));

   ASSERT_NEAR(fie.evaluate(-0.1), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.25), 0.5, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.5), 1.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.75), 0.5, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.1), 0.0, Epsilon);
}


TEST(HierarchicalBasisFunction1D, Basis_2_1)
{
   const  HierarchicalBasisFunction1D fie(HierarchicalLevelIndex(2, 1));

   ASSERT_NEAR(fie.evaluate(-0.1), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.25), 1.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.5), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.75), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.1), 0.0, Epsilon);
}


TEST(HierarchicalBasisFunction1D, Basis_2_3)
{
   const  HierarchicalBasisFunction1D fie(HierarchicalLevelIndex(2, 3));

   ASSERT_NEAR(fie.evaluate(-0.1), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.25), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.5), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(0.75), 1.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.1), 0.0, Epsilon);
}

TEST(HierarchicalBasisFunction1D, Basis_3_5)
{
   const  HierarchicalBasisFunction1D fie(HierarchicalLevelIndex(3, 5));

   ASSERT_NEAR(fie.evaluate(0.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(4.0 / 8.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(4.5 / 8.0), 0.5, Epsilon);
   ASSERT_NEAR(fie.evaluate(5.0 / 8.0), 1.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(5.5 / 8.0), 0.5, Epsilon);
   ASSERT_NEAR(fie.evaluate(6.0 / 8.0), 0.0, Epsilon);
   ASSERT_NEAR(fie.evaluate(1.0), 0.0, Epsilon);
}


