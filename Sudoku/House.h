#pragma once

#include "Cell.h"
#include "SudokuDefines.h"
#include <span>

namespace Sudoku
{

struct House
{
   static House getFromCellaAndType(const Cell &, HouseType);
   static std::span<const House> getAllHouses();
   static std::string toString(HouseType);
   static std::string toString(const House &);

   auto operator<=>(const House &) const = default;

   HouseType houseType;
   Index houseIndex = IndexUndefined;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::House &);
