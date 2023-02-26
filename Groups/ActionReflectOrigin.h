#pragma once

#include "Groups/IFinitePointGroupAction.h"
#include "Hyperplane.h"
#include "Groups/DihedralGroup.h"
#include "PointReflection.h"

template<int N>
class ActionReflectOrigin : public IFinitePointGroupAction<N>
{
public:
   ActionReflectOrigin();
   const IFiniteGroup& getGroup() const override;
   Point<double,N> operator()(int, const Point<double, N>&) const override;

private:
   std::unique_ptr<IFiniteGroup> m_group;
};

template<int N>
ActionReflectOrigin<N>::ActionReflectOrigin() :
   m_group(DihedralGroup::Create(1))
{
}

template<int N>
const IFiniteGroup& ActionReflectOrigin<N>::getGroup() const
{
   return *m_group;
}

template<int N>
Point<double, N> ActionReflectOrigin<N>::operator()(int n, const Point<double, N>& p) const
{
   if (n == 0) return p;
   if (n == 1) return PointReflection<double, N>()(p);
   throw MyException("ActionReflect1D::operator() unknown group element " + std::to_string(n));
}