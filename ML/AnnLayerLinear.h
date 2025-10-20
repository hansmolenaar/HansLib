#pragma once

#include "AnnLayerUniform.h"

namespace ML
{
class AnnLayerLinear : public ML::AnnLayerUniform
{
  public:
    AnnLayerLinear(size_t dim);

  private:
    size_t m_size;
};
} // namespace ML
