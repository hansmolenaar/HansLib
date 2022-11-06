#include "Groups/ActionReflect1D.h" 

ActionReflect1D::ActionReflect1D() :
   m_plane(Point1{0}, UnitVector<1>::Create(Point1{ 1 })),
   m_group(DihedralGroup::Create(1))
{
}


const IFiniteGroup& ActionReflect1D::getGroup() const
{
   return *m_group;
}

Point1 ActionReflect1D::operator()(int n, const Point1& p) const
{
   if (n == 0) return p;
   if (n == 1) return m_plane.reflect(p);
   throw MyException("ActionReflect1D::operator() unknown group element " + std::to_string(n));  
}