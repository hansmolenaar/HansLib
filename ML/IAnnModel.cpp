#include "IAnnModel.h"
#include "Defines.h"
#include "MyAssert.h"
#include "FeedForwardResult.h"

#include <algorithm>

void ML::IAnnModel::checkDimensions() const
{
   const auto layers = getLayers();
   const auto averages = getWeightedAverages();

   Utilities::MyAssert(layers.size() == averages.size());
   for (size_t layer = 0; layer < layers.size(); ++layer)
   {
      Utilities::MyAssert(averages[layer]->getNumberOfNeuronsCur() == layers[layer]->getNumberOfNeurons());
   }
}

std::unique_ptr<ML::IFeedForwardResult> ML::IAnnModel::feedForward(std::span<const double> input, const ML::IParameterSet& parameterSet) const
{
   auto result = std::make_unique< ML::FeedForwardResult>(getLayerDimensions());

   const auto layers = getLayers();
   const auto averages = getWeightedAverages();

   Utilities::MyAssert(input.size() == averages.front()->getNumberOfNeuronsPrv());
   Utilities::MyAssert(averages.size() == parameterSet.getNumLayers());

   // Initialize
   averages.front()->transform(input, parameterSet.at(0), result->setWeightedInputAt(0));
   layers.front()->applyActivatorFunction(result->getWeightedInputAt(0), result->setOutputAt(0));

   // Propagate
   for (size_t n = 1; n < layers.size(); ++n)
   {
      averages[n]->transform(result->getOutputAt(n - 1), parameterSet.at(n), result->setWeightedInputAt(n));
      layers[n]->applyActivatorFunction(result->getWeightedInputAt(n), result->setOutputAt(n));
   }
   return std::unique_ptr<ML::IFeedForwardResult>(result.release());
}

std::vector<size_t> ML::IAnnModel::getLayerDimensions() const
{
   const auto layers = getLayers();
   std::vector<size_t> result(layers.size());
   str::transform(layers, result.begin(), [](const ML::IAnnLayer* layer) {return layer->getNumberOfNeurons(); });
   return result;
}