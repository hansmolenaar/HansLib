#include "Logging.h"
#include "BitsetUtils.h"
#include "SudokuUtils.h"

#include <ostream>

using namespace Sudoku;

namespace
{
static bool s_doLog = false;
}
void Logging::setDoLog(bool onOff)
{
   s_doLog = onOff;
}

bool Logging::getDoLog()
{
   return s_doLog;
}

std::ostream &operator<<(std::ostream &os, const Sudoku::Logging &logging)
{
   os << logging.strategyName << ": " << logging.info.str() << " -> ";
   os << logging.numChanges << " candidate" << (logging.numChanges == 1 ? "" : "s") << " removed\n";
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

std::ostream &operator<<(std::ostream &os, const FieldBits &fields)
{
   os << "(";
   bool first = true;
   for (Field field : SudokuUtils::FieldBitsToRange(fields))
   {
      os << (first ? "" : " ");
      first = false;
      os << Cell::get(field);
   }
   os << ")";

   return os;
}

std::ostream &operator<<(std::ostream &os, std::span<const Sudoku::Field> fields)
{
   const FieldBits fieldBits = Utilities::Bitset<FieldBits().size()>::create(fields);
   os << fieldBits;
   return os;
}
