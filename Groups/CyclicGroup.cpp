#include "CyclicGroup.h"

#include "HLUtils/MessageHandler.h"

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

int CyclicGroup::operator()(int n0, int n1) const
{
   MessageHandler::Assert(std::min(n0, n1) >= 0);
   MessageHandler::Assert(std::max(n0, n1) < m_order);
   return (n0 + n1) % m_order;
}

int CyclicGroup::inverse(int n) const
{
   MessageHandler::Assert(n >= 0 && n < m_order);
   return (m_order - n) % m_order;
}

int CyclicGroup::getIdentityElement() const
{
   return 0;
}