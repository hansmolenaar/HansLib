#include "Groups/FinitePointGroupRotation2.h"
#include "Groups/GroupTableCyclic.h"
#include "MatrixVector/MatrixRotation2D.h"
#include "Utils/MessageHandler.h"
#include "Utils/MathConstants.h"

#include <map>
#include <tuple>

namespace {
   std::map<int, FinitePointGroupRotation2> s_instances;
}

FinitePointGroupRotation2::FinitePointGroupRotation2(int order)
{
   MessageHandler::Assert(order > 0);
   for (int n = 0; n < order; ++n)
   {
      const double angle = 2 * MathConstants::PI * n / order;
      m_rotations.push_back(MatrixRotation2D(angle));
   }
}

const IGroupTable& FinitePointGroupRotation2::GroupTable() const
{
   return GroupTableCyclic::Instance(m_rotations.size());
}

const IFinitePointGroupAction& FinitePointGroupRotation2::GetElement(int n) const
{
   return GetRotation(n);
}

const MatrixRotation2D& FinitePointGroupRotation2::GetRotation(int n) const
{
   return m_rotations.at(n);
}

const FinitePointGroupRotation2& FinitePointGroupRotation2::GetInstance(int order)
{
   auto found = s_instances.find(order);
   if (found == s_instances.end())
   {
      // Create instance
      s_instances.insert(std::make_pair(order, FinitePointGroupRotation2(order)));
      found = s_instances.find(order);
   }
   return found->second;
}
