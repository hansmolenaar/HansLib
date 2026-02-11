#pragma once

class IVector
{
  public:
    virtual ~IVector() = default;

    virtual int GetDimension() const = 0;
    virtual double operator()(int) const = 0;
    virtual double &operator()(int) = 0;
};
