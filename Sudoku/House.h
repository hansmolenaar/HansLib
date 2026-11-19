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
   static const std::vector<House> &getAllHouses();
   static const std::array<HouseId, NumHouse> &getAllHouseIds();
   static std::string toString(HouseType);
   static std::string toString(HouseId);

   HouseType houseType;
   Index houseIndex = IndexUndefined;
   HouseId houseId;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, Sudoku::HouseId);
