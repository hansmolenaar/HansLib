#include "AnnLayerLogistic.h"

#include <cmath>

ML::AnnLayerLogistic::AnnLayerLogistic(size_t dim) : ML::AnnLayerUniform(dim, Logistic, LogisticDeriv)
{
}

double ML::AnnLayerLogistic::Logistic(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

double ML::AnnLayerLogistic::LogisticDeriv(double x)
{
    const double fie = Logistic(x);
    return fie * (1.0 - fie);
}