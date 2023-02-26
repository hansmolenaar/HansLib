#pragma once

#include <cassert>
#include "MyException.h"

namespace Utilities
{
   void  MyAssert(bool assertion);
   void  MyAssert(bool assertion, std::string msg);
}