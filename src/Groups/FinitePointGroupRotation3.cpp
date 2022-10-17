#include "Groups/FinitePointGroupRotation3.h"
#include "Groups/GroupTableCyclic.h"
#include "MatrixVector/MatrixRotation3D.h"
#include "Utils/MessageHandler.h"
#include "Utils/MathConstants.h"


FinitePointGroupRotation3::FinitePointGroupRotation3(int order, const UnitVector3& axis)
{
   MessageHandler::Assert(order > 0);
   for (int n = 0; n < order; ++n)
   {
      const double angle = 2 * MathConstants::PI * n / order;
      m_rotations.push_back(MatrixRotation3D::Create(axis, angle));
   }
}

const IGroupTable& FinitePointGroupRotation3::GroupTable() const
{
   return GroupTableCyclic::Instance(m_rotations.size());
}

const IFinitePointGroupAction& FinitePointGroupRotation3::GetElement(int n) const
{
   return GetRotation(n);
}

const MatrixRotation3D& FinitePointGroupRotation3::GetRotation(int n) const
{
   return m_rotations.at(n);
}

