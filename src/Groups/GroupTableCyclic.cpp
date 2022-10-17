#include "Groups/GroupTableCyclic.h"

#include "Utils/MessageHandler.h"

#include <map>

namespace {
   std::map<int, GroupTableCyclic > s_instances;

   void CheckElement(const GroupTableCyclic& table, int n)
   {

      MessageHandler::Assert(n >= 0 && n < table.Order());
   }
}

GroupTableCyclic::GroupTableCyclic(int order) :
   m_order(order)
{
   MessageHandler::Assert(m_order > 0);
}


const GroupTableCyclic& GroupTableCyclic::Instance(int order)
{
   auto found = s_instances.find(order);
   if (found == s_instances.end())
   {
      s_instances.insert(std::make_pair(order, GroupTableCyclic(order)));
      found = s_instances.find(order);
   }
   return found->second;

}

int GroupTableCyclic::Order() const
{
   return m_order;
}

int GroupTableCyclic::Identity() const
{
   return 0;
}

int GroupTableCyclic::Inverse(int n) const
{
   CheckElement(*this, n);
   return (m_order - n) % m_order;
}

int  GroupTableCyclic::operator()(int n0, int n1) const
{
   CheckElement(*this, n0);
   CheckElement(*this, n1);
   return (n0 + n1) % m_order;
}