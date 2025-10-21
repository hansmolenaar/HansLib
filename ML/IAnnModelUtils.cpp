#include "IAnnModelUtils.h"

#include "AnnArray.h"
#include "Defines.h"
#include "FeedForwardResult.h"
#include "IAnnDataSet.h"
#include "MyAssert.h"
#include "ParameterSet.h"

void ML::IAnnModelUtils::checkDimensions(const ML::IAnnModel &model)
{
    const auto layers = model.getLayers();
    const auto averages = model.getWeights();

    Utilities::MyAssert(layers.size() == averages.size());
    for (size_t layer = 0; layer < layers.size(); ++layer)
    {
        Utilities::MyAssert(averages[layer]->getNumberOfNeuronsCur() == layers[layer]->getNumberOfNeurons());
    }
}

std::unique_ptr<ML::IFeedForwardResult> ML::IAnnModelUtils::feedForward(const IAnnModel &model,
                                                                        std::span<const double> input,
                                                                        const ML::IParameterSet &parameterSet)
{
    auto result = std::make_unique<ML::FeedForwardResult>(input, getLayerDimensions(model));

    const auto layers = model.getLayers();
    const auto weightsAll = model.getWeights();

    Utilities::MyAssert(input.size() == weightsAll.front()->getNumberOfNeuronsPrv());
    Utilities::MyAssert(weightsAll.size() == parameterSet.getNumLayers());

    // Initialize
    weightsAll.front()->setActivation(input, parameterSet.at(0), result->getActivationAtModifiable(0));
    layers.front()->setActivatorFunction(result->getActivationAt(0), result->getOutputAtModifiable(0));

    // Propagate
    for (size_t n = 1; n < layers.size(); ++n)
    {
        weightsAll[n]->setActivation(result->getOutputAt(n - 1), parameterSet.at(n),
                                     result->getActivationAtModifiable(n));
        layers[n]->setActivatorFunction(result->getActivationAt(n), result->getOutputAtModifiable(n));
    }
    return std::unique_ptr<ML::IFeedForwardResult>(result.release());
}

std::vector<size_t> ML::IAnnModelUtils::getLayerDimensions(const ML::IAnnModel &model)
{
    const auto layers = model.getLayers();
    std::vector<size_t> result(layers.size());
    str::transform(layers, result.begin(), [](const ML::IAnnLayer *layer) { return layer->getNumberOfNeurons(); });
    return result;
}

double ML::IAnnModelUtils::getCost(const IAnnModel &model, const ML::IAnnDataSet &dataSet,
                                   const ML::IParameterSet &parameterSet)
{
    std::vector<std::unique_ptr<ML::IFeedForwardResult>> evaluations;
    std::vector<std::span<const double>> actuals;
    for (size_t n = 0; n < dataSet.getNumberOfSamples(); ++n)
    {
        const auto input = dataSet.getNthInput(n);
        evaluations.emplace_back(ML::IAnnModelUtils::feedForward(model, input, parameterSet));
        actuals.emplace_back(evaluations.back()->getOutput());
    }
    return model.getCostFunction().calculate(dataSet, actuals);
}

// Single result at a time => stochastic back propagation
void ML::IAnnModelUtils::backPropagation(const IAnnModel &model, const ML::IFeedForwardResult &forwardResult,
                                         std::span<const double> ideal, double learningRate,
                                         ML::IParameterSet &parameterSet)
{
    ML::ParameterSet parameterDerivs = ML::ParameterSet::CreateUsingDimensions(parameterSet);
    setParameterDerivatives(model, forwardResult, ideal, parameterSet, parameterDerivs);
    updateParameters(model, parameterDerivs, learningRate, parameterSet);
}

void ML::IAnnModelUtils::updateParameters(const ML::IAnnModel &model, const ML::IParameterSet &parameterDerivs,
                                          double learningRate, ML::IParameterSet &parameters)
{
    Utilities::MyAssert(parameterDerivs.getNumLayers() == parameters.getNumLayers());
    const size_t nLlayer = parameters.getNumLayers();
    for (size_t layer = 0; layer < nLlayer; ++layer)
    {
        const auto derivAtLayer = parameterDerivs.at(layer);
        auto params = parameters.getModifiable(layer);
        Utilities::MyAssert(derivAtLayer.size() == params.size());
        std::transform(derivAtLayer.begin(), derivAtLayer.end(), params.begin(), params.begin(),
                       [learningRate](double d, double p) { return p - learningRate * d; });
    }
}

namespace
{
void TimesActivationFunctionDeriv(const ML::IAnnLayer &layerCur, std::span<const double> activationCur,
                                  std::vector<double> &activationDerivWork, std::span<double> errorCur)
{
    const size_t numNeuronsCur = layerCur.getNumberOfNeurons();
    Utilities::MyAssert(activationCur.size() == numNeuronsCur);
    Utilities::MyAssert(errorCur.size() == numNeuronsCur);

    activationDerivWork.resize(numNeuronsCur);
    layerCur.setActivatorFunctionDeriv(activationCur, activationDerivWork);
    std::transform(errorCur.begin(), errorCur.end(), activationDerivWork.begin(), errorCur.begin(), std::multiplies());
}
} // namespace

void ML::IAnnModelUtils::setParameterDerivatives(const ML::IAnnModel &model,
                                                 const ML::IFeedForwardResult &forwardResult,
                                                 std::span<const double> ideal, const ML::IParameterSet &parameters,
                                                 ML::IParameterSet &parameterDerivs)
{
    const auto dimensions = ML::IAnnModelUtils::getLayerDimensions(model);
    const auto maxDim = *str::max_element(dimensions);
    Utilities::MyAssert(dimensions.back() == ideal.size());
    Utilities::MyAssert(forwardResult.getOutput().size() == ideal.size());
    Utilities::MyAssert(parameters.getNumLayers() == dimensions.size());
    Utilities::MyAssert(parameterDerivs.getNumLayers() == dimensions.size());

    const auto layers = model.getLayers();
    ML::AnnArray neuronError(dimensions);
    std::vector<double> activationDerivWork(maxDim);

    setErrors(model, forwardResult, ideal, parameters, neuronError);

    // Initialize
    size_t layer = dimensions.size();
    while (layer > 0)
    {
        --layer;
        const auto errorCurLayer = neuronError.getValues(layer);
        const auto outputPrv = (layer > 0 ? forwardResult.getOutputAt(layer - 1) : forwardResult.getInput());
        model.getWeights()[layer]->backpropagateParamDeriv(errorCurLayer, outputPrv,
                                                           parameterDerivs.getModifiable(layer));
    }
}

void ML::IAnnModelUtils::setErrors(const IAnnModel &model, const ML::IFeedForwardResult &forwardResult,
                                   std::span<const double> ideal, const ML::IParameterSet &parameters,
                                   ML::AnnArray &neuronError)
{
    const auto dimensions = ML::IAnnModelUtils::getLayerDimensions(model);
    const auto maxDim = *str::max_element(dimensions);
    Utilities::MyAssert(parameters.getNumLayers() == dimensions.size());
    Utilities::MyAssert(neuronError.getNumLayers() == dimensions.size());

    const auto layers = model.getLayers();
    std::vector<double> activationDerivWork(maxDim);

    // Initialize
    size_t layer = dimensions.size() - 1;
    const auto outputLayerSize = dimensions.back();
    auto errorOutputLayer = neuronError.getValuesMutable(layer);
    const auto actual = forwardResult.getOutputAt(layer);
    Utilities::MyAssert(errorOutputLayer.size() == outputLayerSize);
    Utilities::MyAssert(actual.size() == outputLayerSize);
    Utilities::MyAssert(ideal.size() == outputLayerSize);
    std::transform(actual.begin(), actual.end(), ideal.begin(), errorOutputLayer.begin(), std::minus());
    TimesActivationFunctionDeriv(*layers[layer], forwardResult.getActivationAt(layer), activationDerivWork,
                                 errorOutputLayer);

    while (layer > 0)
    {
        --layer;
        const auto errorNxtLayer = neuronError.getValues(layer + 1);
        const auto errorCurLayer = neuronError.getValuesMutable(layer);
        model.getWeights()[layer + 1]->backpropagateError(errorNxtLayer, parameters.at(layer + 1), errorCurLayer);
        TimesActivationFunctionDeriv(*layers[layer], forwardResult.getActivationAt(layer), activationDerivWork,
                                     errorCurLayer);
    }
}