#pragma once

#include "AnnLayerUniform.h"

namespace ML
{
class AnnLayerLogistic : public ML::AnnLayerUniform
{
  public:
    AnnLayerLogistic(size_t dim);

    static double Logistic(double);
    static double LogisticDeriv(double);

  private:
    size_t m_size;
};
} // namespace ML
