#pragma once

#include "SudokuDefines.h"

#include <span>

namespace Sudoku
{
struct Cell
{
   Cell() = default;
   explicit Cell(Field);

   Field field = FieldUndefined;
   Index row = IndexUndefined;
   Index col = IndexUndefined;
   Index box = IndexUndefined;

   auto operator<=>(const Cell &) const = default;
   Index operator[](HouseType) const;

   static std::span<const Cell> getAll();
   static const Cell &get(Field);
   static const Cell &getFromRowCol(Index, Index);
   static Field rowCol2Field(Index, Index);

   static const Cell &Rotate(const Cell &);
   static const Cell &ReflectX(const Cell &);
   static const Cell &ReflectY(const Cell &);
   static const Cell &ReflectXY(const Cell &);
   static const Cell &ReflectYX(const Cell &);
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::Cell &);
