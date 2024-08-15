#include <gtest/gtest.h>
#include <array>

#include "AnnLayerLinear.h"
#include "AnnWeightedAverageMatrix.h"
#include "AnnWeightedAverageSingleBias.h"
#include "AnnWeightedAverageWithBias.h"
#include "ParameterSet.h"
#include "AnnCostFunctionSE.h"
#include "AnnModel.h"
#include "Single.h"
#include "AnnDataSet.h"

TEST(IAnnModelTest, FeedForwardBasic)
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

   const auto dims = model.getLayerDimensions();
   ASSERT_EQ(dims.size(), 1);
   ASSERT_EQ(dims[0], 1);

   auto result = model.feedForward(std::vector<double>{0.1}, parameterSet);
   auto output = result->getOutput();
   ASSERT_DOUBLE_EQ(Utilities::Single(output), 0.05);

   result = model.feedForward(std::vector<double>{0.3}, parameterSet);
   output = result->getOutput();
   ASSERT_DOUBLE_EQ(Utilities::Single(output), 0.15);
}


TEST(IAnnModelTest, FeedForwardBasicError)
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

   ASSERT_DOUBLE_EQ(model.calculateError(dataSet, parameterSet), 0.0825);
}



TEST(IAnnModelTest, FeedForwardSlightlyMoreComplex)
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

   ASSERT_DOUBLE_EQ(model.calculateError(dataSet, parameterSet), 0.0625895);
}
