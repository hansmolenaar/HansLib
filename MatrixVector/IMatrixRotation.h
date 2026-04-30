#pragma once

#include "IFinitePointGroupAction.h"
#include "IMatrixSquare.h"
#include "MatrixDense.h"

class IMatrixKelvinRepr;

class IMatrixRotation : public IMatrixSquare
{
  public:
    virtual ~IMatrixRotation() = default;

    void set(int, int, double) override;

    static bool IsRotation(const IMatrixSquare &matrix, double tol = 1.0e-12);
    void RotateTensor(const IMatrixKelvinRepr &, IMatrixKelvinRepr &) const;
};
