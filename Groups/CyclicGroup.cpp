#include "CyclicGroup.h"

#include "Utilities/MessageHandler.h"
#include "IFiniteGroupUtils.h"

std::unique_ptr<CyclicGroup> CyclicGroup::Create(int order)
{
   return std::unique_ptr<CyclicGroup>(new CyclicGroup(order));
}


CyclicGroup::CyclicGroup(int order) : m_order(order)
{
   MessageHandler::Assert(m_order > 0);
}

int CyclicGroup::getOrder() const
{
   return m_order;
}

GroupElement CyclicGroup::operator()(GroupElement n0, GroupElement n1) const
{
   IFiniteGroupUtils::CheckValid(*this, n0);
   IFiniteGroupUtils::CheckValid(*this, n1);
   return (n0 + n1) % m_order;
}

GroupElement CyclicGroup::getInverse(GroupElement n) const
{
   IFiniteGroupUtils::CheckValid(*this, n);
   return (m_order - n) % m_order;
}

GroupElement CyclicGroup::getIdentity() const
{
   return 0;
}