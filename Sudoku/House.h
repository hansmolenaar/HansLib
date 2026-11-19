#pragma once

#include "Cell.h"
#include "SudokuDefines.h"
#include <vector>

namespace Sudoku
{

struct HouseId
{
   HouseType houseType;
   Index houseIndex = IndexUndefined;
   static HouseId getFromCellaAndType(const Cell &, HouseType);

   auto operator<=>(const HouseId &) const = default;
};

struct House
{
   static House getFromCellaAndType(const Cell &, HouseType);
   static const std::vector<House> &getAllHouses();
   static std::string toString(HouseType);
   static std::string toString(const House &);

   auto operator<=>(const House &) const = default;

   HouseType houseType;
   Index houseIndex = IndexUndefined;
   HouseId houseId;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::House &);
