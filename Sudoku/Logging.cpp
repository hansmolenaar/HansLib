#include "Logging.h"
#include "SudokuUtils.h"

#include <ostream>

using namespace Sudoku;

std::ostream &operator<<(std::ostream &os, const Sudoku::Logging &logging)
{
   os << logging.strategyName << ": " << logging.info << " -> " << logging.numChanges << " candidates removed\n";
   return os;
}

std::ostream &operator<<(std::ostream &os, const Sudoku::ValueBits &values)
{
   os << "(";
   bool first = true;
   for (Value value : SudokuUtils::ValueBitsToRange(values))
   {
      os << (first ? "" : " ");
      first = false;
      os << value;
   }
   os << ")";

   return os;
}

std::ostream &operator<<(std::ostream &os, std::span<const Sudoku::Field> fields)
{
   os << "(";
   bool first = true;
   for (Field field : fields)
   {
      os << (first ? "" : " ");
      first = false;
      os << Cell::get(field);
   }
   os << ")";

   return os;
}
