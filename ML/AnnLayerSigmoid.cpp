#include "AnnLayerSigmoid.h"

#include <cmath>

ML::AnnLayerSigmoid::AnnLayerSigmoid(size_t dim) : ML::AnnLayerUniform(dim, Sigmoid, SigmoidDeriv)
{
}

double ML::AnnLayerSigmoid::Sigmoid(double x)
{
   return 1.0 / (1.0 + std::exp(-x));
}


double ML::AnnLayerSigmoid::SigmoidDeriv(double x)
{
   const double fie = Sigmoid(x);
   return fie * (1.0 - fie);
}