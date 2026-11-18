#pragma once

#include "Cell.h"
#include "SudokuDefines.h"
#include <array>
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
   static const House &getRow(Index);
   static const House &getCol(Index);
   static const House &getBox(Index);
   static const House &getHouse(HouseId);
   static const std::vector<House> &getAllHouses();
   static const std::array<HouseId, NumHouse> &getAllHouseIds();
   static std::string toString(HouseType);
   static std::string toString(HouseId);

   HouseId houseId;
   std::array<Cell, NumIndex> cells;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, Sudoku::HouseId);
