#pragma once

#include "DihedralGroup.h"
#include "Hyperplane.h"
#include "IFinitePointGroupAction.h"
#include "PointReflection.h"

template<size_t N>
class ActionReflectOrigin : public IFinitePointGroupAction<N>
{
public:
   ActionReflectOrigin();
   const IFiniteGroup& getGroup() const override;
   Point<double,N> operator()(int, const Point<double, N>&) const override;

private:
   std::unique_ptr<IFiniteGroup> m_group;
};

template<size_t N>
ActionReflectOrigin<N>::ActionReflectOrigin() :
   m_group(DihedralGroup::Create(1))
{
}

template<size_t N>
const IFiniteGroup& ActionReflectOrigin<N>::getGroup() const
{
   return *m_group;
}

template<size_t N>
Point<double, N> ActionReflectOrigin<N>::operator()(int n, const Point<double, N>& p) const
{
   if (n == 0) return p;
   if (n == 1) return PointReflection<double, N>()(p);
   throw MyException("ActionReflect1D::operator() unknown group element " + std::to_string(n));
}
