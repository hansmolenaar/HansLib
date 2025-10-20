#include "AnnCostFunctionMSE.h"
#include "AnnCostFunctionSE.h"
#include "IAnnDataSet.h"
#include "MyAssert.h"

double ML::AnnCostFunctionMSE::calculateSingleSample(std::span<const double> ideal,
                                                     std::span<const double> actual) const
{
    return AnnCostFunctionSE().calculateSingleSample(ideal, actual);
}

double ML::AnnCostFunctionMSE::calculate(const IAnnDataSet &ideal, std::span<std::span<const double>> actual) const
{
    Utilities::MyAssert(!actual.empty());
    return AnnCostFunctionSE().calculate(ideal, actual) / actual.size();
}

double ML::AnnCostFunctionMSE::getScaleFactor(const ML::IAnnDataSet &dataSet) const
{
    const auto nSamples = dataSet.getNumberOfSamples();
    Utilities::MyAssert(nSamples > 0);
    return 1.0 / nSamples;
}