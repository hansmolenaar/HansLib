#pragma once

#include "IMatrix.h"

class IMatrixSquare : public IMatrix
{
  public:
    virtual ~IMatrixSquare() = default;

    virtual int GetDimension() const = 0;

    inline int GetRowDimension() const override;
    inline int GetColDimension() const override;
};

int IMatrixSquare::GetRowDimension() const
{
    return GetDimension();
}

int IMatrixSquare::GetColDimension() const
{
    return GetDimension();
}
