#pragma once

#include "CyclicGroup.h"
#include "IFinitePointGroupAction.h"
#include "Rotate2D.h"

class ActionRotate2D : public IFinitePointGroupAction<2>
{
public:
   ActionRotate2D(int);
   const IFiniteGroup& getGroup() const override;
   Point2 operator()(int, const Point2&) const override;

private:
   std::vector<Rotate2D> m_transformations;
   std::unique_ptr<CyclicGroup> m_group;
};
