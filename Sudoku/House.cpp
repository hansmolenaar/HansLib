#include "House.h"
#include "MyAssert.h"
#include "SudokuDefines.h"

#include <ostream>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace Sudoku;
using namespace Utilities;

namespace
{
std::vector<House> getHousesBy(HouseType houseType)
{
   std::vector<House> housesByIdx(NumIndex);
   std::array<std::vector<Cell>, NumIndex> idx2cell;
   for (Field f : FieldAll)
   {
      const auto &cell = Cell::get(f);
      idx2cell.at(cell[houseType]).push_back(cell);
   }

   for (auto idx : IndexAll)
   {
      MyAssert(idx2cell.at(idx).size() == NumIndex);
      House house;
      house.houseType = houseType;
      house.houseIndex = idx;
      house.houseId = {houseType, idx};
      housesByIdx.at(idx) = house;
   }

   return housesByIdx;
}

const House &getRow(Index row)
{
   static std::vector<House> housesByRow;
   if (housesByRow.empty())
   {
      housesByRow = getHousesBy(HouseType::Row);
   }
   return housesByRow.at(row);
}

const House &getCol(Index col)
{
   static std::vector<House> housesByCol;
   if (housesByCol.empty())
   {
      housesByCol = getHousesBy(HouseType::Col);
   }
   return housesByCol.at(col);
}

const House &getBox(Index box)
{
   static std::vector<House> housesByBox;
   if (housesByBox.empty())
   {
      housesByBox = getHousesBy(HouseType::Box);
   }
   return housesByBox.at(box);
}

const House &getHouse(HouseId houseId)
{
   if (houseId.houseType == HouseType::Row)
   {
      return getRow(houseId.houseIndex);
   }
   else if (houseId.houseType == HouseType::Col)
   {
      return getCol(houseId.houseIndex);
   }
   else
   {
      MyAssert(houseId.houseType == HouseType::Box);
      return getBox(houseId.houseIndex);
   }
}

} // namespace
const std::vector<House> &House::getAllHouses()
{
   static std::vector<House> housesAll;
   if (housesAll.empty())
   {
      for (auto type : HouseTypeAll)
      {
         for (Index idx : IndexAll)
         {
            housesAll.push_back(getHouse({type, idx}));
         }
      }
      MyAssert(housesAll.size() == NumHouse);
   }
   return housesAll;
}
#if false
const std::array<HouseId, NumHouse> &House::getAllHouseIds()
{
   constexpr static std::array<HouseId, NumHouse> s_allHouseIds = {
       HouseId{HouseType::Row, 0}, HouseId{HouseType::Row, 1}, HouseId{HouseType::Row, 2},
       HouseId{HouseType::Row, 3}, HouseId{HouseType::Row, 4}, HouseId{HouseType::Row, 5},
       HouseId{HouseType::Row, 6}, HouseId{HouseType::Row, 7}, HouseId{HouseType::Row, 8},

       HouseId{HouseType::Col, 0}, HouseId{HouseType::Col, 1}, HouseId{HouseType::Col, 2},
       HouseId{HouseType::Col, 3}, HouseId{HouseType::Col, 4}, HouseId{HouseType::Col, 5},
       HouseId{HouseType::Col, 6}, HouseId{HouseType::Col, 7}, HouseId{HouseType::Col, 8},

       HouseId{HouseType::Box, 0}, HouseId{HouseType::Box, 1}, HouseId{HouseType::Box, 2},
       HouseId{HouseType::Box, 3}, HouseId{HouseType::Box, 4}, HouseId{HouseType::Box, 5},
       HouseId{HouseType::Box, 6}, HouseId{HouseType::Box, 7}, HouseId{HouseType::Box, 8}};
   return s_allHouseIds;
}
#endif

HouseId HouseId::getFromCellaAndType(const Cell &cell, HouseType type)
{
   switch (type)
   {
   case HouseType::Row:
      return {type, cell.row};
   case HouseType::Col:
      return {type, cell.col};
   case HouseType::Box:
      return {type, cell.box};
   default:
      throw std::runtime_error("Should not come here");
   }
}

House House::getFromCellaAndType(const Cell &cell, HouseType type)
{
   switch (type)
   {
   case HouseType::Row:
      return {type, cell.row, {type, cell.row}};
   case HouseType::Col:
      return {type, cell.col, {type, cell.col}};
   case HouseType::Box:
      return {type, cell.box, {type, cell.box}};
   default:
      throw std::runtime_error("Should not come here");
   }
}

std::string House::toString(HouseType houseType)
{
   if (houseType == HouseType::Row)
   {
      return "ROW";
   }
   else if (houseType == HouseType::Col)
   {
      return "COL";
   }
   else
   {
      return "BOX";
   }
}
std::string House::toString(const House &house)

{
   std::stringstream stream;
   stream << house;
   return stream.str();
}

std::ostream &operator<<(std::ostream &os, const Sudoku::House &house)
{
   os << "[" << House::toString(house.houseType) << " " << house.houseIndex << "]";
   return os;
}
