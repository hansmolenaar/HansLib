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

struct CellInHouse
{
   bool operator()(const Cell &) const;
   HouseId houseId;
};

struct House
{
   static const House &getRow(Index);
   static const House &getCol(Index);
   static const House &getBox(Index);
   static const House &getHouse(HouseId);
   static const std::vector<House> &getAllHouses();
   static FieldBits getFieldsInHouse(HouseId);

   HouseId houseId;
   std::array<Cell, NumIndex> cells;
};

} // namespace Sudoku
