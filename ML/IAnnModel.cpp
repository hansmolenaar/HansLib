#include "IAnnModel.h"
#include "Defines.h"
#include "MyAssert.h"
#include "FeedForwardResult.h"
#include "IAnnDataSet.h"
#include "AnnArray.h"

#include <algorithm>

namespace
{
   void SetActivationDerivAt()
   {

   }
}
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

double ML::IAnnModel::calculateError(const ML::IAnnDataSet& dataSet, const ML::IParameterSet& parameterSet) const
{
   std::vector<std::unique_ptr<ML::IFeedForwardResult>> evaluations;
   std::vector<std::span<const double>> actuals;
   for (size_t n = 0; n < dataSet.getNumberOfSamples(); ++n)
   {
      const auto input = dataSet.getNthInput(n);
      evaluations.emplace_back(feedForward(input, parameterSet));
      actuals.emplace_back(evaluations.back()->getOutput());
   }
   return getCostFunction().calculate(dataSet, actuals);
}


// Single result at a time => stochastic back propagation
void ML::IAnnModel::backPropagation(const ML::IFeedForwardResult& forwardResult, std::span<const double> ideal, double learningRate, ML::IParameterSet& parameterSet) const
{
   const auto dimensions = getLayerDimensions();
   const auto maxDim = *str::max_element(dimensions);
   Utilities::MyAssert(dimensions.back() == ideal.size());
   Utilities::MyAssert(forwardResult.getOutput().size() == ideal.size());
   Utilities::MyAssert(parameterSet.getNumLayers() == dimensions.size());

   const auto layers = getLayers();
   AnnArray neuronError(dimensions);
   std::vector<double> activationDeriv(maxDim);

   // Initialize
   size_t layer = dimensions.size() - 1;
   auto errorOutputLayer = neuronError.modifyValuesAt(layer);
   const auto actual = forwardResult.getOutputAt(layer);
   activationDeriv.resize(dimensions.back());
   layers.back()->applyActivatorFunctionDeriv(forwardResult.getWeightedInputAt(layer), activationDeriv);
   for (size_t n = 0; n < dimensions.back(); ++n)
   {
      errorOutputLayer[n] = (actual[n] - ideal[n]) * activationDeriv.at(n);
   }

   std::vector<double> check(parameterSet.at(layer).size());

   Utilities::MyAssert(layer > 0);
   getWeightedAverages().back()->backpropInit(forwardResult.getOutputAt(layer - 1), errorOutputLayer, check);
}