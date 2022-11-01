#pragma once

#include <string>
#include <sstream>

template<typename T>
std::string ToString(T value)
{
   std::ostringstream os;
   os << value;
   return os.str();
}
