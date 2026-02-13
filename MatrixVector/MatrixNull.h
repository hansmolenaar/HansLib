#pragma once

#include "IMatrix.h"

class MatrixNull : public IMatrix
{
  public:
    int GetRowDimension() const override;
    int GetColDimension() const override;

    double operator()(int, int) const override;
    void set(int, int, double) override;

    void timesVector(std::span<const double>, std::span<double>) const override;
};
