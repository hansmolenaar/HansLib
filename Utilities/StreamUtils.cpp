#include "StreamUtils.h"

std::ostream &operator<<(std::ostream &os, std::span<const std::string> strings)
{
   for (const auto &s : strings)
   {
      os << s + "\n";
   }
   return os;
}
