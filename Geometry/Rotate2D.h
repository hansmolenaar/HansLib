#pragma once

#include "IPointTransformation.h"
#include "UnitVector.h"

class Rotate2D : public IPointTransformation<double, 2>
{
  public:
    ~Rotate2D() override = default;
    explicit Rotate2D(double);
    Point2 operator()(const Point2 &) const override;
    UnitVector<double, 2> operator()(const UnitVector<double, 2> &) const;

  private:
    double m_angle;
    double m_cos;
    double m_sin;
};