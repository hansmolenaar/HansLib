#include "AnnWeightsNoBias.h"
#include "Defines.h"
#include "MyAssert.h"

#include <numeric>

ML::AnnWeightsNoBias::AnnWeightsNoBias(size_t dimPrv, size_t dimCur) : m_layerSizePrv(dimPrv), m_layerSizeCur(dimCur)
{
}

size_t ML::AnnWeightsNoBias::getNumberOfNeuronsPrv() const
{
    return m_layerSizePrv;
}

size_t ML::AnnWeightsNoBias::getNumberOfNeuronsCur() const
{
    return m_layerSizeCur;
}

size_t ML::AnnWeightsNoBias::getNumberOfParameters() const
{
    return m_layerSizePrv * m_layerSizeCur;
}

void ML::AnnWeightsNoBias::setActivation(std::span<const double> outputPrv, std::span<const double> params,
                                         std::span<double> activation) const
{
    Utilities::MyAssert(outputPrv.size() == m_layerSizePrv);
    Utilities::MyAssert(activation.size() == m_layerSizeCur);
    Utilities::MyAssert(params.size() == getNumberOfParameters());

    const double *paramsIter = params.data();
    for (size_t cur = 0; cur < m_layerSizeCur; ++cur)
    {
        activation[cur] = std::inner_product(outputPrv.begin(), outputPrv.end(), paramsIter, 0.0);
        paramsIter += m_layerSizePrv;
    }
}

void ML::AnnWeightsNoBias::backpropagateError(std::span<const double> errorCur, std::span<const double> params,
                                              std::span<double> errorPrv) const
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

void ML::AnnWeightsNoBias::backpropagateParamDeriv(std::span<const double> errorCur, std::span<const double> outputPrv,
                                                   std::span<double> dCost_dParam) const
{
    Utilities::MyAssert(outputPrv.size() == m_layerSizePrv);
    Utilities::MyAssert(errorCur.size() == m_layerSizeCur);
    Utilities::MyAssert(dCost_dParam.size() == getNumberOfParameters());

    size_t pos = 0;
    for (size_t neuronCur = 0; neuronCur < m_layerSizeCur; ++neuronCur)
    {
        for (size_t neuronPrv = 0; neuronPrv < m_layerSizePrv; ++neuronPrv)
        {
            dCost_dParam[pos] = errorCur[neuronCur] * outputPrv[neuronPrv];
            ++pos;
        }
    }
}