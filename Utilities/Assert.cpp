#include "Assert.h"

void  Utilities::Assert(bool assertion)
{
   if (!assertion)
   {
      throw MyException("Assertion failuer");
   }
}

void  Utilities::Assert(bool assertion, std::string msg)
{
   if (!assertion)
   {
      throw MyException(msg);
   }
}
