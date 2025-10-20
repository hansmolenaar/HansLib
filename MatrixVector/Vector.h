#pragma once

#include "IVector.h"
#include <vector>

class Vector : public IVector
{
  public:
    Vector(int);
    int GetDimension() const override;
    double operator()(int) const override;
    double &operator()(int) override;
    double *data();

  private:
    std::vector<double> m_data;
};
