#include "MyAssert.h"

void  Utilities::MyAssert(bool assertion)
{
   if (!assertion)
   {
      throw MyException("Assertion failuer");
   }
}

void  Utilities::MyAssert(bool assertion, std::string msg)
{
   if (!assertion)
   {
      throw MyException(msg);
   }
}
