#include <gtest/gtest.h>

#include "AnnCostFunctionMSE.h"
#include "AnnDataSet.h"
#include "MyException.h"

TEST(AnnCostFunctionMSETest, Basic)
{
    const ML::AnnCostFunctionMSE costFunction;
    ML::AnnDataSet dataSet(2, 1);
    dataSet.addSample({1, 2}, {3});
    dataSet.addSample({4, 5}, {6});

    const std::initializer_list<double> result1{-1};
    const std::vector<double> result2{1};
    std::vector<std::span<const double>> result{result1, result2};
    const double cost = costFunction.calculate(dataSet, result);
    ASSERT_DOUBLE_EQ(cost, 41.0 / 4);
    ASSERT_DOUBLE_EQ(costFunction.getScaleFactor(dataSet), 0.5);
}
