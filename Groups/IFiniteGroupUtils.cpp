#include "HLUtils/Defines.h"
#include "HLUtils/MessageHandler.h"
#include "IFiniteGroupUtils.h"
#include "IFiniteGroup.h"

void IFiniteGroupUtils::CheckGroupAxioms(const IFiniteGroup& group)
{
   const int order = group.getOrder();
   MessageHandler::Assert(order > 0);

   // Is closed
   for (int n0 = 0; n0 < order; ++n0)
   {
      for (int n1 = 0; n1 < order; ++n1)
      {
         int result = group(n0, n1);
         MessageHandler::Assert(result >= 0 && result < order);
      }
   }

   //  identity
   const int identity = group.getIdentityElement();
   for (int n = 0; n < order; ++n)
   {
      MessageHandler::Assert(group(identity, n) == n);
      MessageHandler::Assert(group(n, identity) == n);
   }

   // inverse
   for (int n = 0; n < order; ++n)
   {
      const int inverse = group.inverse(n);
      MessageHandler::Assert(group(inverse, n) == identity);
      MessageHandler::Assert(group(n, inverse) == identity);
   }

   // Associativity, should inplement Light's algorithm
   for (int n0 = 0; n0 < order; ++n0)
   {
      for (int n1 = 0; n1 < order; ++n1)
      {
         for (int n2 = 0; n2 < order; ++n2)
         {
            const int res1 = group(group(n0, n1), n2);
            const int res2 = group(n0, group(n1, n2));
            MessageHandler::Assert(res1 == res2);
         }
      }
   }

}