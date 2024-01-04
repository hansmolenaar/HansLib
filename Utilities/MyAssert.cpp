#include "MyAssert.h"

void  Utilities::MyAssert(bool assertion)
{
   MyAssert(assertion, "Assertion failure");
}

void  Utilities::MyAssert(bool assertion, std::string msg)
{
   if (!assertion)
   {
      throw MyException(msg);
   }
}
