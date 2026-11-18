#pragma once

#include "SudokuDefines.h"

#include <set>

namespace Sudoku
{

struct FieldValue
{
   Field field = FieldUndefined;
   Value value = ValueUndefined;
   using Container = std::set<FieldValue>;
   static void add(const FieldBits &, Value, Container &);
   static void add(const Container &, Container &);

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
