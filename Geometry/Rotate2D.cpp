#include "Rotate2D.h"
#include <cmath>

Rotate2D::Rotate2D(double angle) : m_angle(angle), m_cos(std::cos(m_angle)), m_sin(std::sin(m_angle))
{}

Point2 Rotate2D::operator()(const Point2& point) const
{
   return Point2{ 
      m_cos * point[0] - m_sin * point[1],
      m_sin * point[0] + m_cos * point[1] };
}
