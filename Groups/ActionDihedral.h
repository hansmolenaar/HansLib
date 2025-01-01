#pragma once

#include "DihedralGroup.h"
#include "IFinitePointGroupAction.h"
#include "IPointTransformation.h"

class ActionDihedral : public IFinitePointGroupAction<2>
{
public:
   ActionDihedral(int);
   ActionDihedral(int, const Point2&);
   const IFiniteGroup& getGroup() const override;
   Point2 operator()(int, const Point2&) const override;

private:
   std::vector<std::unique_ptr<IPointTransformation<double, 2>>> m_transformations;
   std::unique_ptr<IFiniteGroup> m_group;
};
