#include "HLUtils/Defines.h"
#include "HLUtils/MessageHandler.h"
#include "IFiniteGroupUtils.h"
#include "IFiniteGroup.h"

void IFiniteGroupUtils::CheckValid(const IFiniteGroup& group, GroupElement elm)
{
   MessageHandler::Assert(elm >= 0);
   MessageHandler::Assert(elm < group.getOrder());
}

std::vector<GroupElement> IFiniteGroupUtils::GetElements(const IFiniteGroup& group)
{
   const auto order = group.getOrder();
   std::vector < GroupElement> result(order);
   for (auto n = 0; n < order; ++n)
   {
      result.at(n) = n;
   }
   return result;
}

void IFiniteGroupUtils::CheckGroupAxioms(const IFiniteGroup& group)
{
   const int order = group.getOrder();
   MessageHandler::Assert(order > 0);

   const auto& elements = GetElements(group);

   // Is closed
   for (auto n0 : elements)
   {
      for (auto n1 : elements)
      {
         int result = group(n0, n1);
         CheckValid(group, result);
      }
   }

   //  identity
   const auto identity = group.getIdentityElement();
   for (auto n : elements)
   {
      MessageHandler::Assert(group(identity, n) == n);
      MessageHandler::Assert(group(n, identity) == n);
   }

   // inverse
   for (auto n : elements)
   {
      const int inverse = group.inverse(n);
      MessageHandler::Assert(group(inverse, n) == identity);
      MessageHandler::Assert(group(n, inverse) == identity);
   }

   // Associativity, should inplement Light's algorithm
   for (auto n0  : elements)
   {
      for (auto n1 : elements)
      {
         for (auto n2 : elements)
         {
            const auto res1 = group(group(n0, n1), n2);
            const auto res2 = group(n0, group(n1, n2));
            MessageHandler::Assert(res1 == res2);
         }
      }
   }

}