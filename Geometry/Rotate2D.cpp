#include "Rotate2D.h"
#include <cmath>

Rotate2D::Rotate2D(double angle) : m_angle(angle), m_cos(std::cos(m_angle)), m_sin(std::sin(m_angle))
{}

 std::unique_ptr<IPointTransformation<double, 2>> Rotate2D::Create(double angle)
{
    return std::make_unique<Rotate2D>(angle);
}

Point2 Rotate2D::operator()(const Point2& point) const
{
   return Point2{ 
      m_cos * point.data()[0] - m_sin * point.data()[1],
      m_sin * point.data()[0] + m_cos * point.data()[1] };
}

std::unique_ptr< UnitVector<double, 2>> Rotate2D::operator()(const UnitVector<double, 2>& uv) const
{
   const Point2 point{ uv[0], uv[1] };
   const Point2 rotated = (*this)(point);
   return UnitVector<double, 2>::Create(rotated);
}