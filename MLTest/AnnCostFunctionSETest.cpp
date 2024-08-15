#include <gtest/gtest.h>

#include "AnnCostFunctionSE.h" 
#include "AnnDataSet.h" 
#include "MyException.h"

TEST(AnnCostFunctionSETest, Basic)
{
   const ML::AnnCostFunctionSE costFunction;
   ML::AnnDataSet dataSet(2, 1);
   ASSERT_EQ(dataSet.getNumberOfSamples(), 0);
   dataSet.addSample({ 1, 2 }, { 3 });
   dataSet.addSample({ 4, 5 }, { 6 });

   const std::initializer_list<double> result1{ -1 };
   const std::vector<double> result2{ 1 };
   std::vector<std::span<const double>> result{ result1, result2 };
   const double cost = costFunction.calculate(dataSet, result);
   ASSERT_DOUBLE_EQ(cost, 41.0 / 2);
}


TEST(AnnCostFunctionSETest, Single)
{
   const ML::AnnCostFunctionSE costFunction;
   ASSERT_DOUBLE_EQ(costFunction.calculateSingleSample({}, {}), 0);
   ASSERT_DOUBLE_EQ(costFunction.calculateSingleSample(std::vector<double>{ -1.0 }, std::vector<double>{ 1.0 }), 2.0);
   ASSERT_DOUBLE_EQ(costFunction.calculateSingleSample(std::vector<double>{ -1, -2 }, std::vector<double>{ 1, 2 }), 10.0);
}


TEST(AnnCostFunctionSETest, Errors)
{
   const ML::AnnCostFunctionSE costFunction;
   ML::AnnDataSet dataSet(2, 1);
   std::vector<std::span<const double>> result;
   ASSERT_THROW(costFunction.calculate(dataSet, result), MyException);

   const std::vector<double> result1{ 1, 2 };
   result.emplace_back(result1);
   ASSERT_THROW(costFunction.calculate(dataSet, result), MyException);

   dataSet.addSample({ 1, 2 }, { 3 });
   ASSERT_THROW(costFunction.calculate(dataSet, result), MyException);
}
