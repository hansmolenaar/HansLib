#include "ActionDihedral.h"
#include "Rotate2D.h"
#include "UnitVector.h"
#include "Hyperplane.h"

#include <numbers>

ActionDihedral::ActionDihedral(int numVertices) : ActionDihedral(numVertices, Point2{ 1,0 })
{
}

ActionDihedral::ActionDihedral(int numVertices, const Point2& point) :
   m_group(DihedralGroup::Create(numVertices))
{
   if (numVertices < 1) throw MyException("ActionDihedral::ActionDihedral unexpected size " + std::to_string(numVertices));
   m_transformations.reserve(2 * numVertices);
   for (int n = 0; n < numVertices; ++n)
   {
      const double angle = n * 2 * std::numbers::pi / numVertices;
      m_transformations.emplace_back(std::unique_ptr<IPointTransformation<double, 2>>(new Rotate2D(angle)));
   }
   const auto uv = UnitVector<double,2>::Create(Point2{ point.data()[1], -point.data()[0] });
   if (!uv) throw MyException("ActionDihedral::ActionDihedral 0 is not a valid reference point");
   for (int n = 0; n < numVertices; ++n)
   {
      const double angle = n * std::numbers::pi / numVertices;
      auto rotated = Rotate2D(angle)(*uv);
      std::unique_ptr<IPointTransformation<double, 2>> plane = std::make_unique<Hyperplane<double,2>>(Point2{}, std::move(rotated));
      m_transformations.push_back(std::move(plane));
   }
}

const IFiniteGroup& ActionDihedral::getGroup() const
{
   return *m_group;
}


Point2 ActionDihedral::operator()(int n, const Point2& point) const
{
   return (*m_transformations.at(n))(point);
}