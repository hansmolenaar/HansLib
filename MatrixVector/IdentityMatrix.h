#pragma once

#include "IMatrixSymmetric.h"
#include <span>

class IdentityMatrix : public IMatrixSymmetric
{
  public:
    explicit IdentityMatrix(int);
    static const IdentityMatrix &GetInstance(int);
    int GetDimension() const override;
    double get(int, int) const override;
    void set(int, int, double) override;
    void timesVector(std::span<const double>, std::span<double>) const override;

  private:
    int m_dimension;
};
