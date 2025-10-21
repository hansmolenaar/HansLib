#pragma once

#include "MatrixKelvinReprBase.h"
#include <array>
#include <span>

class IMatrixSymmetric;

class MatrixKelvinRepr3 : public MatrixKelvinReprBase<3>
{
  public:
    static MatrixKelvinRepr3 CreateEmpty();
    static MatrixKelvinRepr3 Create(const IMatrixSymmetric &);
    static MatrixKelvinRepr3 CreateFromVector(std::span<const double> values);
    static MatrixKelvinRepr3 CreateIdentity();

  private:
    MatrixKelvinRepr3();
};

MatrixKelvinRepr3 operator*(double, const MatrixKelvinRepr3 &);
MatrixKelvinRepr3 operator+(const MatrixKelvinRepr3 &, const MatrixKelvinRepr3 &);
MatrixKelvinRepr3 operator-(const MatrixKelvinRepr3 &, const MatrixKelvinRepr3 &);