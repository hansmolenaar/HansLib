#include "AnnWeightedAverageWithBias.h"
#include "MyAssert.h"
#include "Defines.h"

ML::AnnWeightedAverageWithBias::AnnWeightedAverageWithBias(size_t dimPrv, size_t dimCur) : m_matrixOnly(dimPrv, dimCur)
{
}

size_t ML::AnnWeightedAverageWithBias::getNumberOfNeuronsPrv() const
{
   return m_matrixOnly.getNumberOfNeuronsPrv();
}

size_t ML::AnnWeightedAverageWithBias::getNumberOfNeuronsCur() const
{
   return m_matrixOnly.getNumberOfNeuronsCur();
}

size_t ML::AnnWeightedAverageWithBias::getNumberOfParameters() const
{
   return m_matrixOnly.getNumberOfParameters() + m_matrixOnly.getNumberOfNeuronsCur();
}

void ML::AnnWeightedAverageWithBias::transform(std::span<const double> activatorValuesPrv, std::span<const double> params, std::span<double> weightedAverage) const
{
   Utilities::MyAssert(params.size() == getNumberOfParameters());
   const size_t matrixSize = m_matrixOnly.getNumberOfParameters();
   m_matrixOnly.transform(activatorValuesPrv, std::span<const double>(params.begin(), matrixSize), weightedAverage);
   const std::span<const double> bias(params.begin() + matrixSize, getNumberOfNeuronsCur());
   std::transform(weightedAverage.begin(), weightedAverage.end(), bias.begin(), weightedAverage.begin(), [](double x, double y) {return x + y; });
}


void ML::AnnWeightedAverageWithBias::backpropInit(std::span<const double> activatorValuesPrv, std::span<const double> dError_dWeightedAverageLast, std::span<double> dError_dParam) const
{
   Utilities::MyAssert(dError_dParam.size() == getNumberOfParameters());
   const size_t numMatrixParams = m_matrixOnly.getNumberOfParameters();
   m_matrixOnly.backpropInit(activatorValuesPrv, dError_dWeightedAverageLast, std::span<double>(dError_dParam.begin(), numMatrixParams));
   std::copy(dError_dWeightedAverageLast.begin(), dError_dWeightedAverageLast.end(), dError_dParam.begin() + numMatrixParams);
}


void ML::AnnWeightedAverageWithBias::backpropagateError(std::span<const double> errorCur, std::span<const double> params, std::span<double> errorPrv) const
{
   m_matrixOnly.backpropagateError(errorCur, std::span<const double>(params.begin(), m_matrixOnly.getNumberOfParameters()), errorPrv);
}


void ML::AnnWeightedAverageWithBias::backpropagateParamDeriv(std::span<const double> errorCur, std::span<const double> activatorValuesPrv, std::span<double> dError_dParam) const
{
   str::fill(dError_dParam, 0.0);
   m_matrixOnly.backpropagateError(errorCur, activatorValuesPrv, std::span< double>(dError_dParam.begin(), m_matrixOnly.getNumberOfParameters()));
}