#pragma once

#include "MyException.h"
#include <cassert>

namespace Utilities
{
   void  MyAssert(bool assertion);
   void  MyAssert(bool assertion, std::string msg);
}