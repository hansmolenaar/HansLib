#include "AnnModel.h"

#include "MyAssert.h"

ML::AnnModel::AnnModel(std::span<const ML::IAnnLayer*> layers, std::span<const ML::IAnnWeightedAverage*> weightedAverages, const ML::IAnnCostFunction& costFunction) :
   m_layers(layers), m_weightedAverages(weightedAverages), m_costFunction(costFunction)
{
   checkDimensions();
}

std::span<const ML::IAnnLayer*> ML::AnnModel::getLayers() const
{
   return m_layers;
}

std::span<const ML::IAnnWeightedAverage*> ML::AnnModel::getWeightedAverages() const
{
   return m_weightedAverages;
}

const ML::IAnnCostFunction& ML::AnnModel::getCostFunction() const
{
   return m_costFunction;
}