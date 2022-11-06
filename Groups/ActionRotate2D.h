#pragma once

#include "Groups/IFinitePointGroupAction.h"
#include "Geometry/Rotate2D.h"

class ActionRotate2D : public IFinitePointGroupAction<2>
{
public:
   ActionRotate2D(int);
   const IFiniteGroup& getGroup() const override;
   Point2 operator()(int, Point2) const override;

private:
   std::vector<Rotate2D> m_transformations;
};
