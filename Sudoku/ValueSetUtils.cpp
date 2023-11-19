
#include "ValueSetUtils.h"
#include "Functors.h"
#include "MyException.h"
#include "Defines.h"

using namespace Sudoku;

bool ValueSetUtils::IsSolved(const ValueSet& valueSet)
{
   bool result = true;
   const Functors::IsInIncludeBounds inBounds{ValueAll.front(), ValueAll.back()};
   std::bitset<NumRowColBoxPositions> isSet;
   for (auto value : valueSet)
   {
      if (value == ValueUndefined)
      {
         result = false;
         continue;
      }

      if (!inBounds(value))
      {
         throw MyException("ValueSetIsSolved() value out of bounds: " + std::to_string(value));
      }

      if (isSet.test(value - 1))
      {
         throw MyException("ValueSetIsSolved() value " + std::to_string(value) + " is set twice");
      }
      isSet.set(value - 1);
   }
   return result;
}