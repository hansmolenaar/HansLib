#pragma once

#include "IMatrixSymmetric.h"
#include <span>

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
    virtual ~IdentityMatrix() {};
    void timesVector(std::span<const double>, std::span<double>) const;

  private:
    int m_dimension;
};
