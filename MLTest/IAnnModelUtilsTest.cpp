#include <gtest/gtest.h>
#include <array>

#include "IAnnModelUtils.h"
#include "AnnLayerLinear.h"
#include "AnnLayerLogistic.h"
#include "AnnWeightsNoBias.h"
#include "AnnWeightsSingleBias.h"
#include "AnnWeightsWithBias.h"
#include "ParameterSet.h"
#include "AnnCostFunctionSE.h"
#include "AnnModel.h"
#include "Single.h"
#include "AnnDataSet.h"
#include "Functors.h"


TEST(IAnnModelUtilsTest, FeedForwardBasic)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightsSingleBias averageWeight(1, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeight };

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

   const ML::AnnWeightsWithBias averageWeight(1, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::AnnDataSet dataSet(1, 1);
   dataSet.addSample({ 0.1 }, { 0.2 });
   dataSet.addSample({ 0.3 }, { 0.25 });
   dataSet.addSample({ 0.6 }, { 0.4 });
   dataSet.addSample({ 0.7 }, { 0.7 });

   ASSERT_DOUBLE_EQ(ML::IAnnModelUtils::getCost(model, dataSet, parameterSet), 0.0825);
}

TEST(IAnnModelUtilsTest, FeedForwardSlightlyMoreComplex)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightsSingleBias averageWeight(2, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.1, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::AnnDataSet dataSet(2, 1);
   dataSet.addSample({ 0.1, 0.1 * 0.1 }, { 0.2 });
   dataSet.addSample({ 0.3, 0.3 * 0.3 }, { 0.25 });
   dataSet.addSample({ 0.6, 0.6 * 0.6 }, { 0.4 });
   dataSet.addSample({ 0.7, 0.7 * 0.7 }, { 0.7 });

   ASSERT_DOUBLE_EQ(ML::IAnnModelUtils::getCost(model, dataSet, parameterSet), 0.0625895);
}


TEST(IAnnModelUtilsTest, FeedForwardWithHiddenLayer)
{
   // See https://hmkcode.com/ai/backpropagation-step-by-step/
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear hiddenLayer(2);
   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &hiddenLayer, &outputLayer };

   const ML::AnnWeightsNoBias averageWeightHidden(2, 2);
   const ML::AnnWeightsNoBias averageWeightOutput(2, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeightHidden, &averageWeightOutput };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.11, 0.21, 0.12, 0.08 });
   parameterSet.add({ 0.14, 0.15 });

   const ML::AnnModel model(layers, matrices, costFunction);

   ML::AnnDataSet dataSet(2, 1);
   dataSet.addSample({ 2, 3 }, { 1 });

   const double expect = 0.5 * std::pow((1 - 0.191), 2);
   ASSERT_DOUBLE_EQ(ML::IAnnModelUtils::getCost(model, dataSet, parameterSet), expect);
}

TEST(IAnnModelUtilsTest, SetParameterDerivativesSuperSimple)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightsSingleBias averageWeight(1, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeight };

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

   const ML::AnnWeightsSingleBias averageWeight(2, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &averageWeight };

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

   const ML::AnnWeightsNoBias weightHidden(2, 2);
   const ML::AnnWeightsNoBias weightOutput(2, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &weightHidden, &weightOutput };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.2, 0.2, 0.3, 0.3 });
   parameterSet.add({ 0.3, 0.9 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto forwardResult = ML::IAnnModelUtils::feedForward(model, std::vector<double>{0.35, 0.7}, parameterSet);
   ASSERT_DOUBLE_EQ(Utilities::Single(forwardResult->getOutput()), 0.66507363952475640);

   const std::vector<double> ideal{ 0.5 };

   ML::AnnArray neuronError(ML::IAnnModelUtils::getLayerDimensions(model));
   ML::IAnnModelUtils::setErrors(model, *forwardResult, ideal, parameterSet, neuronError);

   const Functors::AreClose areClose(1.0e-10); // Values in article?
   ASSERT_TRUE(areClose(neuronError.getValuesAt(0)[0], 0.0027275877458127758));
   ASSERT_TRUE(areClose(neuronError.getValuesAt(0)[1], 0.0080714273160845482));
   ASSERT_TRUE(areClose(neuronError.getValuesAt(1)[0], 0.036770267688329382));

   constexpr double learningRate = 1;
   ML::IAnnModelUtils::backPropagation(model, *forwardResult, ideal, learningRate, parameterSet);

}


TEST(IAnnModelUtilsTest, NeuroticExample)
{
   // See https://theneuralblog.com/forward-pass-backpropagation-example/
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLogistic hiddenLayer(2);
   const ML::AnnLayerLogistic outputLayer(2);
   std::vector<const ML::IAnnLayer*> layers{ &hiddenLayer, &outputLayer };

   const ML::AnnWeightsSingleBias weightHidden(2, 2);
   const ML::AnnWeightsSingleBias weightOutput(2, 2);
   std::vector<const ML::IAnnWeights*> matrices{ &weightHidden, &weightOutput };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.1, 0.3, 0.2, 0.4, 0.25 });
   parameterSet.add({ 0.5, 0.6, 0.7, 0.8, 0.35 });

   ML::AnnDataSet dataSet(2, 2);
   dataSet.addSample({ 0.1, 0.5 }, { 0.05, 0.95 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto forwardResult = ML::IAnnModelUtils::feedForward(model, dataSet.getNthInput(0), parameterSet);

   const Functors::AreClose areClose(8.0e-3); // Rounded numbers in article
   ASSERT_TRUE(areClose(forwardResult->getOutput()[0], 0.73492));
   ASSERT_TRUE(areClose(forwardResult->getOutput()[1], 0.77955));

   const double error = ML::IAnnModelUtils::getCost(model, dataSet, parameterSet);
   ASSERT_TRUE(areClose(error, 0.24908));

   ML::ParameterSet parameterDerivs = ML::ParameterSet::CreateUsingDimensions(parameterSet);
   ML::IAnnModelUtils::setParameterDerivatives(model, *forwardResult, dataSet.getNthOutput(0), parameterSet, parameterDerivs);
   ASSERT_TRUE(areClose(parameterDerivs.at(1)[0], 0.08020));
   ASSERT_TRUE(areClose(parameterDerivs.at(1)[1], 0.08211));
   ASSERT_TRUE(areClose(parameterDerivs.at(1)[2], -0.01760));
   ASSERT_TRUE(areClose(parameterDerivs.at(1)[3], -0.01802));
   ASSERT_TRUE(areClose(parameterDerivs.at(0)[0], 0.00110));


   constexpr double learningRate = 0.6;
   ML::IAnnModelUtils::backPropagation(model, *forwardResult, dataSet.getNthOutput(0), learningRate, parameterSet);

   ASSERT_TRUE(areClose(parameterSet.at(1)[0], 0.45187));
   ASSERT_TRUE(areClose(parameterSet.at(1)[1], 0.55073));
   ASSERT_TRUE(areClose(parameterSet.at(1)[2], 0.71056));
   ASSERT_TRUE(areClose(parameterSet.at(1)[3], 0.81081));

   ASSERT_TRUE(areClose(parameterSet.at(0)[0], 0.09933));
   ASSERT_TRUE(areClose(parameterSet.at(0)[1], 0.29667));
   ASSERT_TRUE(areClose(parameterSet.at(0)[2], 0.19919));
   ASSERT_TRUE(areClose(parameterSet.at(0)[3], 0.39597));
}

TEST(IAnnModelUtilsTest, BuggedBiasedExample)
{
   // See https://medium.com/@karna.sujan52/back-propagation-algorithm-numerical-solved-f60c6986b643
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLogistic hiddenLayer(2);
   const ML::AnnLayerLogistic outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &hiddenLayer, &outputLayer };

   const ML::AnnWeightsWithBias weightHidden(2, 2);
   const ML::AnnWeightsWithBias weightOutput(2, 1);
   std::vector<const ML::IAnnWeights*> matrices{ &weightHidden, &weightOutput };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, -0.3, 0.2, 0.5, 0.6, -0.4 });
   parameterSet.add({ 0.1, 0.3, 0.8 });

   ML::AnnDataSet dataSet(2, 1);
   dataSet.addSample({ 1, 1 }, { 0 });
   dataSet.addSample({ 0, 1 }, { 1 });

   const ML::AnnModel model(layers, matrices, costFunction);
   auto forwardResult = ML::IAnnModelUtils::feedForward(model, dataSet.getNthInput(0), parameterSet);

   const Functors::AreClose areClose(1.0e-3);
   ASSERT_TRUE(areClose(Utilities::Single(forwardResult->getOutput()), 0.7391));

   ML::AnnArray neuronError(ML::IAnnModelUtils::getLayerDimensions(model));
   ML::IAnnModelUtils::setErrors( model, *forwardResult, dataSet.getNthOutput(0), parameterSet, neuronError);

   ASSERT_TRUE(areClose(neuronError.getValuesAt(0)[1], 0.010451));
   ASSERT_TRUE(areClose(neuronError.getValuesAt(0)[0], 0.0030481));
   ASSERT_TRUE(areClose(neuronError.getValuesAt(1)[0], 0.1425));
}