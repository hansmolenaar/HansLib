#pragma once

#include "IMatrix.h"

class MatrixNull : public IMatrix
{
  public:
    int GetRowDimension() const override;
    int GetColDimension() const override;

    double operator()(int, int) const override;
    double &operator()(int, int) override;

    std::vector<double> timesVector(const std::vector<double> &) const;
};
