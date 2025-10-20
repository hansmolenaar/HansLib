#pragma once

#include "IVector.h"

#include <span>

class IIndexer;

class VectorWrapperRO : public IVector
{
  public:
    explicit VectorWrapperRO(std::span<const double>);
    int GetDimension() const override;
    double operator()(int) const override;
    double &operator()(int) override;

  private:
    std::span<const double> m_data;
};
