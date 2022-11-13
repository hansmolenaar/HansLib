#pragma once

#include "MyException.h"

namespace Utilities
{
   void  Assert(bool assertion);
   void  Assert(bool assertion, std::string msg);
}
