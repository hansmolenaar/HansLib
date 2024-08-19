#include <gtest/gtest.h>
#include <array>

#include "IAnnModelUtils.h"
#include "AnnLayerLinear.h"
#include "AnnLayerLogistic.h"
#include "AnnWeightedAverageMatrix.h"
#include "AnnWeightedAverageSingleBias.h"
#include "AnnWeightedAverageWithBias.h"
#include "ParameterSet.h"
#include "AnnCostFunctionSE.h"
#include "AnnModel.h"
#include "Single.h"
#include "AnnDataSet.h"

TEST(IAnnModelUtilsTest, FeedForwardBasic)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightedAverageSingleBias averageWeight(1, 1);
   std::vector<const ML::IAnnWeightedAverage*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);

   const auto dims = ML::IAnnModelUtils::getLayerDimensions(model);
   ASSERT_EQ(dims.size(), 1);
   ASSERT_EQ(dims[0], 1);

   auto result = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.1}, parameterSet);
   auto output = result->getOutput();
   ASSERT_DOUBLE_EQ(Utilities::Single(output), 0.05);

   result = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.3}, parameterSet);
   output = result->getOutput();
   ASSERT_DOUBLE_EQ(Utilities::Single(output), 0.15);
}


TEST(IAnnModelUtilsTest, FeedForwardBasicError)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightedAverageWithBias averageWeight(1, 1);
   std::vector<const ML::IAnnWeightedAverage*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::AnnDataSet dataSet(1, 1);
   dataSet.addSample({ 0.1 }, { 0.2 });
   dataSet.addSample({ 0.3 }, { 0.25 });
   dataSet.addSample({ 0.6 }, { 0.4 });
   dataSet.addSample({ 0.7 }, { 0.7 });

   ASSERT_DOUBLE_EQ(ML::IAnnModelUtils::calculateError(model, dataSet, parameterSet), 0.0825);
}

TEST(IAnnModelUtilsTest, FeedForwardSlightlyMoreComplex)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightedAverageSingleBias averageWeight(2, 1);
   std::vector<const ML::IAnnWeightedAverage*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.1, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::AnnDataSet dataSet(2, 1);
   dataSet.addSample({ 0.1, 0.1 * 0.1 }, { 0.2 });
   dataSet.addSample({ 0.3, 0.3 * 0.3 }, { 0.25 });
   dataSet.addSample({ 0.6, 0.6 * 0.6 }, { 0.4 });
   dataSet.addSample({ 0.7, 0.7 * 0.7 }, { 0.7 });

   ASSERT_DOUBLE_EQ(ML::IAnnModelUtils::calculateError(model, dataSet, parameterSet), 0.0625895);
}


TEST(IAnnModelUtilsTest, FeedForwardWithHiddenLayer)
{
   // See https://hmkcode.com/ai/backpropagation-step-by-step/
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear hiddenLayer(2);
   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &hiddenLayer, &outputLayer };

   const ML::AnnWeightedAverageMatrix averageWeightHidden(2, 2);
   const ML::AnnWeightedAverageMatrix averageWeightOutput(2, 1);
   std::vector<const ML::IAnnWeightedAverage*> matrices{ &averageWeightHidden, &averageWeightOutput };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.11, 0.21, 0.12, 0.08 });
   parameterSet.add({ 0.14, 0.15 });

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::AnnDataSet dataSet(2, 1);
   dataSet.addSample({ 2, 3 }, { 1 });

   const double expect = 0.5 * std::pow((1 - 0.191), 2);
   ASSERT_DOUBLE_EQ(ML::IAnnModelUtils::calculateError(model, dataSet, parameterSet), expect);
}

TEST(IAnnModelUtilsTest, SetParameterDerivativesSuperSimple)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightedAverageSingleBias averageWeight(1, 1);
   std::vector<const ML::IAnnWeightedAverage*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto result = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.7}, parameterSet);
   auto derivs = ML::ParameterSet::CreateUsingDimensions(parameterSet);
   ML::IAnnModelUtils::setParameterDerivatives(model, *result, std::vector<double>{0.7}, parameterSet, derivs);
   ASSERT_DOUBLE_EQ(derivs.at(0)[0], -0.245);
   ASSERT_DOUBLE_EQ(derivs.at(0)[1], -0.35);
}


TEST(IAnnModelUtilsTest, SetParameterDerivativesSimple)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightedAverageSingleBias averageWeight(2, 1);
   std::vector<const ML::IAnnWeightedAverage*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.1, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto result = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.7, 0.49}, parameterSet);
   auto derivs = ML::ParameterSet::CreateUsingDimensions(parameterSet);
   ML::IAnnModelUtils::setParameterDerivatives(model, *result, std::vector<double>{0.7}, parameterSet, derivs);
   ASSERT_DOUBLE_EQ(derivs.at(0)[0], -0.2107);
   ASSERT_DOUBLE_EQ(derivs.at(0)[1], -0.14749);
   ASSERT_DOUBLE_EQ(derivs.at(0)[2], -0.301);
}



TEST(IAnnModelUtilsTest, GeeksExample)
{
   // See https://www.geeksforgeeks.org/backpropagation-in-neural-network/
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLogistic hiddenLayer(2);
   const ML::AnnLayerLogistic outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &hiddenLayer, &outputLayer };

   const ML::AnnWeightedAverageMatrix weightHidden(2, 2);
   const ML::AnnWeightedAverageMatrix weightOutput(2, 1);
   std::vector<const ML::IAnnWeightedAverage*> matrices{ &weightHidden, &weightOutput };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.2, 0.2, 0.3, 0.3 });
   parameterSet.add({ 0.3, 0.9 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto forwardResult = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.35, 0.7}, parameterSet);
   ASSERT_DOUBLE_EQ(Utilities::Single(forwardResult->getOutput()), 0.66507363952475640);

   const std::vector<double> ideal{ 0.5 };
   constexpr double learningRate = 1;
   ML::IAnnModelUtils::backPropagation(model, *forwardResult, ideal, learningRate, parameterSet);

}
