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

   // 0 is identity
   for (int n = 0; n < order; ++n)
   {
      MessageHandler::Assert(group(0, n) == n);
      MessageHandler::Assert(group(n, 0) == n);
   }

   // inverse
   for (int n = 0; n < order; ++n)
   {
      const int inverse = group.inverse(n);
      MessageHandler::Assert(group(inverse, n) == 0);
      MessageHandler::Assert(group(n, inverse) == 0);
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