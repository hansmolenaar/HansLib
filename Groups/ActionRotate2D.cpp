#include "ActionRotate2D.h"

#include <numbers>

ActionRotate2D::ActionRotate2D(int order) : m_group(CyclicGroup::Create(order))
{
   if (order < 1) throw MyException("ActionRotate2D::ActionRotate2D unexpected size " + std::to_string(order));
   m_transformations.reserve(order);
   for (int n = 0; n < order; ++n)
   {
      m_transformations.push_back(Rotate2D(n * 2 * std::numbers::pi / order));
   }
}
const IFiniteGroup& ActionRotate2D::getGroup() const
{
   return *m_group;
}

Point2 ActionRotate2D::operator()(int n, const Point2& point) const
{
   return m_transformations.at(n)(point);
}