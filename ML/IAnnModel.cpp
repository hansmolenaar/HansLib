#include "IAnnModel.h"

#include "MyAssert.h"

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
   const auto averages = getWeightedAverages();
   Utilities::MyAssert(input.size() == averages.front()->getNumberOfNeuronsPrv());
   Utilities::MyAssert(averages.size() == parameterSet.getNumLayers());
   return {};
}