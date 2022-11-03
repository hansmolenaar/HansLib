#pragma once

#include "IPointTransformation.h"

class Rotate2D : public IPointTransformation<double, 2>
{
public:
   explicit Rotate2D(double);
   Point2 operator()(const Point2&) const override;
private:
   double m_angle;
   double m_cos;
   double m_sin;
};