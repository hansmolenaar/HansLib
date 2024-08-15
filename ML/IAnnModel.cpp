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