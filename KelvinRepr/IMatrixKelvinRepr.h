#pragma once

#include <span>

class IMatrixKelvinRepr
{
  public:
    virtual int GetDimension() const = 0;
    virtual double Get(int, int) const = 0;
    virtual void Set(int, int, double) = 0;
    virtual std::span<const double> Vector() const = 0;
    virtual ~IMatrixKelvinRepr() {};
};
