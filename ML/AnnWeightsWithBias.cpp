#include "AnnWeightsWithBias.h"
#include "MyAssert.h"
#include "Defines.h"

ML::AnnWeightsWithBias::AnnWeightsWithBias(size_t dimPrv, size_t dimCur) : m_matrixOnly(dimPrv, dimCur)
{
}

size_t ML::AnnWeightsWithBias::getNumberOfNeuronsPrv() const
{
   return m_matrixOnly.getNumberOfNeuronsPrv();
}

size_t ML::AnnWeightsWithBias::getNumberOfNeuronsCur() const
{
   return m_matrixOnly.getNumberOfNeuronsCur();
}

size_t ML::AnnWeightsWithBias::getNumberOfParameters() const
{
   return m_matrixOnly.getNumberOfParameters() + m_matrixOnly.getNumberOfNeuronsCur();
}

void ML::AnnWeightsWithBias::setActivation(std::span<const double> outputPrv, std::span<const double> params, std::span<double> activation) const
{
   Utilities::MyAssert(params.size() == getNumberOfParameters());
   const size_t matrixSize = m_matrixOnly.getNumberOfParameters();
   m_matrixOnly.setActivation(outputPrv, std::span<const double>(params.begin(), matrixSize), activation);
   const std::span<const double> bias(params.begin() + matrixSize, getNumberOfNeuronsCur());
   std::transform(activation.begin(), activation.end(), bias.begin(), activation.begin(), [](double x, double y) {return x + y; });
}


void ML::AnnWeightsWithBias::backpropInit(std::span<const double> activatorValuesPrv, std::span<const double> dError_dWeightedAverageLast, std::span<double> dError_dParam) const
{
   Utilities::MyAssert(dError_dParam.size() == getNumberOfParameters());
   const size_t numMatrixParams = m_matrixOnly.getNumberOfParameters();
   m_matrixOnly.backpropInit(activatorValuesPrv, dError_dWeightedAverageLast, std::span<double>(dError_dParam.begin(), numMatrixParams));
   std::copy(dError_dWeightedAverageLast.begin(), dError_dWeightedAverageLast.end(), dError_dParam.begin() + numMatrixParams);
}


void ML::AnnWeightsWithBias::backpropagateError(std::span<const double> errorCur, std::span<const double> params, std::span<double> errorPrv) const
{
   m_matrixOnly.backpropagateError(errorCur, std::span<const double>(params.begin(), m_matrixOnly.getNumberOfParameters()), errorPrv);
}


void ML::AnnWeightsWithBias::backpropagateParamDeriv(std::span<const double> errorCur, std::span<const double> activatorValuesPrv, std::span<double> dError_dParam) const
{
   str::fill(dError_dParam, 0.0);
   m_matrixOnly.backpropagateParamDeriv(errorCur, activatorValuesPrv, std::span< double>(dError_dParam.begin(), m_matrixOnly.getNumberOfParameters()));

   size_t pos = m_matrixOnly.getNumberOfParameters();
   for (size_t neuronCur = 0; neuronCur < errorCur.size(); ++neuronCur)
   {
      dError_dParam[pos] = errorCur[neuronCur];
      ++pos;
   }
}