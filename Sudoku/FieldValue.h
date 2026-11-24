#pragma once

#include "SudokuDefines.h"

#include <set>
#include <vector>

namespace Sudoku
{

struct FieldValue
{
   Field field = FieldUndefined;
   Value value = ValueUndefined;
   using Container = std::set<FieldValue>;
   static bool add(const FieldBits &, Value, Container &);
   static void add(const Container &, Container &);

   auto operator<=>(const FieldValue &) const = default;
};

class FieldValues
{
 public:
   FieldValueCount add(FieldValue);
   FieldValueCount add(Field, Value);
   FieldValueCount add(const FieldBits &, Value);
   FieldValueCount add(const FieldValues &);
   std::span<const FieldValue> get() const;

 private:
   std::vector<FieldValue> m_fieldValues;
   std::set<FieldValue> m_fieldValuesSorted;
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
