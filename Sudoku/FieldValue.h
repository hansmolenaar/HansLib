#pragma once

#include "SudokuDefines.h"

namespace Sudoku
{

struct FieldValue
{
   Field field = FieldUndefined;
   Value value = ValueUndefined;

   auto operator<=>(const FieldValue &) const = default;
};

} // namespace Sudoku

namespace std
{
template <> struct hash<Sudoku::FieldValue>
{
   size_t operator()(const Sudoku::FieldValue &fv) const noexcept
   {
      // perfect hashing
      return fv.field * Sudoku::NumValue + fv.value;
   }
};
} // namespace std
