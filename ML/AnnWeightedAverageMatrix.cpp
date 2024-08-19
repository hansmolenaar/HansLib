#include "AnnWeightedAverageMatrix.h"
#include "MyAssert.h"
#include "Defines.h"

#include <numeric>

ML::AnnWeightedAverageMatrix::AnnWeightedAverageMatrix(size_t dimPrv, size_t dimCur) : m_layerSizePrv(dimPrv), m_layerSizeCur(dimCur)
{
}

size_t ML::AnnWeightedAverageMatrix::getNumberOfNeuronsPrv() const
{
   return m_layerSizePrv;
}

size_t ML::AnnWeightedAverageMatrix::getNumberOfNeuronsCur() const
{
   return m_layerSizeCur;
}

size_t ML::AnnWeightedAverageMatrix::getNumberOfParameters() const
{
   return m_layerSizePrv * m_layerSizeCur;
}

void ML::AnnWeightedAverageMatrix::transform(std::span<const double> activatorValuesPrv, std::span<const double> params, std::span<double> weightedAverage) const
{
   Utilities::MyAssert(activatorValuesPrv.size() == m_layerSizePrv);
   Utilities::MyAssert(weightedAverage.size() == m_layerSizeCur);
   Utilities::MyAssert(params.size() == getNumberOfParameters());

   const double* paramsIter = params.data();
   for (size_t cur = 0; cur < m_layerSizeCur; ++cur)
   {
      weightedAverage[cur] = std::inner_product(activatorValuesPrv.begin(), activatorValuesPrv.end(), paramsIter, 0.0);
      paramsIter += m_layerSizePrv;
   }
}

void ML::AnnWeightedAverageMatrix::backpropInit(std::span<const double> activatorValuesPrv, std::span<const double> dError_dWeightedAverageLast, std::span<double> dError_dParam) const
{
   Utilities::MyAssert(activatorValuesPrv.size() == m_layerSizePrv);
   Utilities::MyAssert(dError_dWeightedAverageLast.size() == m_layerSizeCur);
   Utilities::MyAssert(dError_dParam.size() == getNumberOfParameters());
   size_t pos = 0;
   for (size_t neuronCur = 0; neuronCur < m_layerSizeCur; ++neuronCur)
   {
      for (size_t neuronPrv = 0; neuronPrv < m_layerSizePrv; ++neuronPrv)
      {
         dError_dParam[pos] = activatorValuesPrv[neuronPrv] * dError_dWeightedAverageLast[neuronCur];
         ++pos;
      }
   }
}

void ML::AnnWeightedAverageMatrix::backpropagateError(std::span<const double> errorCur, std::span<const double> params, std::span<double> errorPrv) const
{
   Utilities::MyAssert(errorPrv.size() == m_layerSizePrv);
   Utilities::MyAssert(errorCur.size() == m_layerSizeCur);
   Utilities::MyAssert(params.size() == getNumberOfParameters());

   str::fill(errorPrv, 0.0);
   size_t pos = 0;
   for (size_t neuronCur = 0; neuronCur < m_layerSizeCur; ++neuronCur)
   {
      for (size_t neuronPrv = 0; neuronPrv < m_layerSizePrv; ++neuronPrv)
      {
         errorPrv[neuronPrv] += params[pos] * errorCur[neuronCur];
         ++pos;
      }
   }
}

void ML::AnnWeightedAverageMatrix::backpropagateParamDeriv(std::span<const double> errorCur, std::span<const double> activatorValuesPrv, std::span<double> dError_dParam) const
{
   Utilities::MyAssert(activatorValuesPrv.size() == m_layerSizePrv);
   Utilities::MyAssert(errorCur.size() == m_layerSizeCur);
   Utilities::MyAssert(dError_dParam.size() == getNumberOfParameters());

   size_t pos = 0;
   for (size_t neuronCur = 0; neuronCur < m_layerSizeCur; ++neuronCur)
   {
      for (size_t neuronPrv = 0; neuronPrv < m_layerSizePrv; ++neuronPrv)
      {
         dError_dParam[pos] = errorCur[neuronCur] * activatorValuesPrv[neuronPrv];
         ++pos;
      }
   }
}