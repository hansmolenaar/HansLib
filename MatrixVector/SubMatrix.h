#pragma once

#include "IMatrix.h"
#include <array>

class SubMatrix : public IMatrix
{
  public:
    SubMatrix(IMatrix &, int, int, int, int);
    int GetRowDimension() const override;
    int GetColDimension() const override;
    double get(int, int) const override;
    void set(int, int, double) override;
    void timesVector(std::span<const double>, std::span<double>) const override;

  private:
    IMatrix &m_matrix;
    std::array<int, 2> m_ofset;
    std::array<int, 2> m_dimension;
};
