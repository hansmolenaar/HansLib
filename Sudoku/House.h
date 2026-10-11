#pragma once

#include "Cell.h"
#include <array>
#include <vector>

namespace Sudoku
{
enum class HouseType
{
   Row,
   Col,
   Box
};

constexpr std::array<HouseType, 3> HouseTypeAll = {HouseType::Row, HouseType::Col, HouseType::Box};

constexpr Index NumHouse = 3 * NumIndex;

struct House
{
   static const House &getRow(Index);
   static const House &getCol(Index);
   static const House &getBox(Index);
   static const House &get(HouseType, Index);
   static const std::vector<House> &getAllHouses();

   std::array<Cell, NumIndex> cells;
};

} // namespace Sudoku
