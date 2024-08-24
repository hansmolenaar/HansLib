#include "AnnWeightsSingleBias.h"
#include "MyAssert.h"

#include <numeric>
#include <algorithm>

ML::AnnWeightsSingleBias::AnnWeightsSingleBias(size_t dimPrv, size_t dimCur) : m_matrixOnly(dimPrv, dimCur)
{
}

size_t ML::AnnWeightsSingleBias::getNumberOfNeuronsPrv() const
{
   return m_matrixOnly.getNumberOfNeuronsPrv();
}

size_t ML::AnnWeightsSingleBias::getNumberOfNeuronsCur() const
{
   return m_matrixOnly.getNumberOfNeuronsCur();
}

size_t ML::AnnWeightsSingleBias::getNumberOfParameters() const
{
   return m_matrixOnly.getNumberOfParameters() + 1;
}

void ML::AnnWeightsSingleBias::setActivation(std::span<const double> outputPrv, std::span<const double> params, std::span<double> activation) const
{
   Utilities::MyAssert(params.size() == getNumberOfParameters());
   m_matrixOnly.setActivation(outputPrv, std::span<const double>(params.begin(), m_matrixOnly.getNumberOfParameters()), activation);
   const double bias = params.back();
   std::transform(activation.begin(), activation.end(), activation.begin(), [bias](double x) {return x + bias; });
}

void ML::AnnWeightsSingleBias::backpropagateError(std::span<const double> errorCur, std::span<const double> params, std::span<double> errorPrv) const
{
   m_matrixOnly.backpropagateError(errorCur, std::span<const double>(params.begin(), m_matrixOnly.getNumberOfParameters()), errorPrv);
}


void ML::AnnWeightsSingleBias::backpropagateParamDeriv(std::span<const double> errorCur, std::span<const double> outputPrv, std::span<double> dCost_dParam) const
{
   m_matrixOnly.backpropagateParamDeriv(errorCur, outputPrv, std::span< double>(dCost_dParam.begin(), m_matrixOnly.getNumberOfParameters()));
   dCost_dParam.back() = std::accumulate(errorCur.begin(), errorCur.end(), 0.0);
}