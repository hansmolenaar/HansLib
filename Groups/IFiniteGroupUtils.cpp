#include "Defines.h"
#include "MyAssert.h"
#include "IFiniteGroupUtils.h"
#include "IFiniteGroup.h"

void IFiniteGroupUtils::CheckValid(const IFiniteGroup& group, GroupElement elm)
{
   Utilities::MyAssert(elm >= 0);
   Utilities::MyAssert(elm < group.getOrder());
}

std::vector<GroupElement> IFiniteGroupUtils::GetElements(const IFiniteGroup& group)
{
   const auto order = group.getOrder();
   std::vector < GroupElement> result(order);
   for (GroupElement n = 0; n < order; ++n)
   {
      result.at(n) = n;
   }
   return result;
}

void IFiniteGroupUtils::CheckGroupAxioms(const IFiniteGroup& group, bool checkAssociativity)
{
   const int order = group.getOrder();
   if (order == 0) return;

   const auto& elements = GetElements(group);

   // Is closed
   for (auto n0 : elements)
   {
      for (auto n1 : elements)
      {
         const auto result = group(n0, n1);
         CheckValid(group, result);
      }
   }

   //  identity
   const auto identity = group.getIdentity();
   for (auto n : elements)
   {
      Utilities::MyAssert(group(identity, n) == n);
      Utilities::MyAssert(group(n, identity) == n);
   }

   // inverse
   for (auto n : elements)
   {
      const auto inverse = group.getInverse(n);
      Utilities::MyAssert(group(inverse, n) == identity);
      Utilities::MyAssert(group(n, inverse) == identity);
   }

   // Associativity, should inplement Light's algorithm
   if (checkAssociativity)
   {
      for (auto n0 : elements)
      {
         for (auto n1 : elements)
         {
            for (auto n2 : elements)
            {
               const auto res1 = group(group(n0, n1), n2);
               const auto res2 = group(n0, group(n1, n2));
               Utilities::MyAssert(res1 == res2);
            }
         }
      }
   }

}