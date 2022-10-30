#include "ToString.h"

#include <sstream>

std::string ToString(size_t value)
{
   std::ostringstream os;
   os << value;
   return os.str();
}