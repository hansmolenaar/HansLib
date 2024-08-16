#include "AnnWeightedAverageMatrix.h"
#include "MyAssert.h"

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

void ML::AnnWeightedAverageMatrix::backpropInit(std::span<const double> activatorValuesPrv, std::span<const double> dError_dWeightedAverageLast, std::span<const double> params,
   std::span<double> dError_dParam) const
{
   throw MyException("Not yet implemented: backpropInit");
}