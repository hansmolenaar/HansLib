#include "AnnModel.h"
#include "IAnnModelUtils.h"

#include "MyAssert.h"

ML::AnnModel::AnnModel(std::span<const ML::IAnnLayer*> layers, std::span<const ML::IAnnWeights*> weights, const ML::IAnnCostFunction& costFunction) :
   m_layers(layers), m_weights(weights), m_costFunction(costFunction)
{
   ML::IAnnModelUtils::checkDimensions(*this);
}

std::span<const ML::IAnnLayer*> ML::AnnModel::getLayers() const
{
   return m_layers;
}

std::span<const ML::IAnnWeights*> ML::AnnModel::getWeights() const
{
   return m_weights;
}

const ML::IAnnCostFunction& ML::AnnModel::getCostFunction() const
{
   return m_costFunction;
}