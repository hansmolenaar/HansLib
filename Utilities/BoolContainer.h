#pragma once

#include <deque>

using BoolContainer = std::deque<bool>;

namespace BoolContainerUtils
{
   BoolContainer FromNumber(int);
   int ToNumber(const BoolContainer&);
}
