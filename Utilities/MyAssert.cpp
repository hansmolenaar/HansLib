#include "MyAssert.h"

void  Utilities::MyAssert(bool assertion)
{
   MyAssert(assertion, "Assertion failuer");
}

void  Utilities::MyAssert(bool assertion, std::string msg)
{
   if (!assertion)
   {
      throw MyException(msg);
   }
}
