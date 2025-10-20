#pragma once

#include "IMatrixSymmetric.h"

class IdentityMatrix : public IMatrixSymmetric
{
  public:
    explicit IdentityMatrix(int);
    static const IdentityMatrix &GetInstance(int);
    virtual int GetDimension() const override;
    virtual int GetRowDimension() const override;
    virtual int GetColDimension() const override;
    virtual double operator()(int, int) const override;
    virtual double &operator()(int, int) override;
    virtual ~IdentityMatrix(){};

  private:
    int m_dimension;
};