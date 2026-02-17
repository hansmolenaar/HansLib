#include <gtest/gtest.h>

#include "Defines.h"
#include "EigenSolution.h"
#include "Single.h"

using namespace Utilities;

TEST(EigenSolutionTest, OnlyEigenValues)
{
    constexpr double eps = 1.0e-12;
    std::vector<double> eigenValues{2, 1, 3};
    const EigenSolution solution(eigenValues);
    ASSERT_FALSE(solution.hasEigenVectors());
    ASSERT_EQ(3, solution.getAvailableEigenValues().size());
    ASSERT_NEAR(solution.getAvailableEigenValues()[0], 1.0, eps);
    ASSERT_NEAR(solution.getAvailableEigenValues()[1], 2.0, eps);
    ASSERT_NEAR(solution.getAvailableEigenValues()[2], 3.0, eps);
}

TEST(EigenSolutionTest, WithEigenVectors)
{
    constexpr double eps = 1.0e-12;
    std::vector<double> eigenValues{2, 1, 3};
    std::vector<std::vector<double>> eigenVectors{{2}, {1}, {3}};

    const EigenSolution solution(eigenValues, eigenVectors);

    ASSERT_TRUE(solution.hasEigenVectors());
    ASSERT_EQ(3, solution.getAvailableEigenValues().size());
    ASSERT_NEAR(solution.getAvailableEigenValues()[0], 1.0, eps);
    ASSERT_NEAR(solution.getAvailableEigenValues()[1], 2.0, eps);
    ASSERT_NEAR(solution.getAvailableEigenValues()[2], 3.0, eps);

    ASSERT_NEAR(Single(solution.getNthEigenVector(0)), 1.0, eps);
    ASSERT_NEAR(Single(solution.getNthEigenVector(1)), 2.0, eps);
    ASSERT_NEAR(Single(solution.getNthEigenVector(2)), 3.0, eps);
}
