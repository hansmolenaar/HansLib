#pragma once

#include <sstream>
#include <string>

template<typename T>
std::string ToString(T value)
{
   std::ostringstream os;
   os << value;
   return os.str();
}
