#pragma once

#include "IFinitePointGroupAction.h"
#include "IMatrixRotation.h"
#include "MatrixDense.h"

#include <map>

class MatrixRotation2D : public IMatrixRotation
{
  public:
    MatrixRotation2D(double);
    int GetDimension() const override;

    double get(int, int) const override;

    void timesVector(std::span<const double>, std::span<double>) const override;

  private:
    MatrixDense m_matrix;
};
