#include "AnnWeightedAverageSingleBias.h"
#include "MyAssert.h"

#include <numeric>
#include <algorithm>

ML::AnnWeightedAverageSingleBias::AnnWeightedAverageSingleBias(size_t dimPrv, size_t dimCur) : m_matrixOnly(dimPrv, dimCur)
{
}

size_t ML::AnnWeightedAverageSingleBias::getNumberOfNeuronsPrv() const
{
   return m_matrixOnly.getNumberOfNeuronsPrv();
}

size_t ML::AnnWeightedAverageSingleBias::getNumberOfNeuronsCur() const
{
   return m_matrixOnly.getNumberOfNeuronsCur();
}

size_t ML::AnnWeightedAverageSingleBias::getNumberOfParameters() const
{
   return m_matrixOnly.getNumberOfParameters() + 1;
}

void ML::AnnWeightedAverageSingleBias::transform(std::span<const double> activatorValuesPrv, std::span<const double> params, std::span<double> weightedAverage) const
{
   Utilities::MyAssert(params.size() == getNumberOfParameters());
   m_matrixOnly.transform(activatorValuesPrv, std::span<const double>(params.begin(), m_matrixOnly.getNumberOfParameters()), weightedAverage);
   const double bias = params.back();
   std::transform(weightedAverage.begin(), weightedAverage.end(), weightedAverage.begin(), [bias](double x) {return x + bias; });
}

void ML::AnnWeightedAverageSingleBias::backpropInit(std::span<const double> activatorValuesPrv, std::span<const double> dError_dWeightedAverageLast, std::span<double> dError_dParam) const
{
   Utilities::MyAssert(dError_dParam.size() == getNumberOfParameters());
   m_matrixOnly.backpropInit(activatorValuesPrv, dError_dWeightedAverageLast, std::span<double>(dError_dParam.begin(), m_matrixOnly.getNumberOfParameters()));
   dError_dParam.back() = std::accumulate(dError_dWeightedAverageLast.begin(), dError_dWeightedAverageLast.end(), 0.0);
}