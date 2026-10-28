#pragma once

#include "Cell.h"
#include <array>
#include <vector>

namespace Sudoku
{

constexpr auto NumCellsInHouse = NumIndex;

enum class HouseType
{
   Row,
   Col,
   Box
};

constexpr std::array<HouseType, 3> HouseTypeAll = {HouseType::Row, HouseType::Col, HouseType::Box};

constexpr Index NumHouse = 3 * NumIndex;

struct HouseId
{
   HouseType houseType;
   Index houseIndex;
};

struct House
{
   static const House &getRow(Index);
   static const House &getCol(Index);
   static const House &getBox(Index);
   static const House &getHouse(HouseId);
   static const std::vector<House> &getAllHouses();

   HouseId houseId;
   std::array<Cell, NumIndex> cells;
};

} // namespace Sudoku
