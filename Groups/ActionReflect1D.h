#pragma once

#include "Groups/IFinitePointGroupAction.h"
#include "Geometry/Hyperplane.h"
#include "Groups/DihedralGroup.h"

class ActionReflect1D : public IFinitePointGroupAction<1>
{
public:
   ActionReflect1D();
   const IFiniteGroup& getGroup() const override;
   Point1 operator()(int, const Point1&) const override;

private:
   Hyperplane<1> m_plane;
   std::unique_ptr<IFiniteGroup> m_group;
};
