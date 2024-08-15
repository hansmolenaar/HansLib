#include <gtest/gtest.h>
#include <array>

#include "AnnLayerLinear.h"
#include "AnnWeightedAverageMatrix.h"
#include "ParameterSet.h"
#include "AnnCostFunctionSE.h"
#include "AnnModel.h"

TEST(IAnnModelTest, FeedForwardBasic)
{
   // See https://towardsdatascience.com/training-a-neural-network-by-hand-1bcac4d82a6e
   const ML::AnnCostFunctionSE costFunction;

   const ML::AnnLayerLinear outputLayer(1);
   std::vector<const ML::IAnnLayer*> layers{ &outputLayer };

   const ML::AnnWeightedAverageMatrix averageWeight(1, 1);
   std::vector<const ML::IAnnWeightedAverage*> matrices{ &averageWeight };

   ML::ParameterSet parameterSet;
   parameterSet.add({ 0.5, 0.0 });

   const ML::AnnModel model(layers, matrices, costFunction);

   model.feedForward(std::vector<double>{0.1}, parameterSet);
}
