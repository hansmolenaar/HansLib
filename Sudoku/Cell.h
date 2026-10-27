#pragma once

#include "SudokuDefines.h"
#include "boost/container/static_vector.hpp"

#include <vector>

namespace Sudoku
{
struct Cell
{
   Cell() = default;
   Cell(Field);

   Field field = FieldUndefined;
   Index row = IndexUndefined;
   Index col = IndexUndefined;
   Index box = IndexUndefined;

   auto operator<=>(const Cell &) const = default;

   static const std::vector<Cell> &getAll();
   static Field getField(const Cell &);
   static const Cell &get(Field);
   static const Cell &getFromRowCol(Index, Index);
   static Field rowCol2Field(Index, Index);
   static bool AreSame(const Cell &cell, Field field);
   static boost::container::static_vector<Field, NumField> cells2fields(std::span<const Cell>);
};

} // namespace Sudoku
