#include "House.h"
#include "MyAssert.h"
#include "SudokuUtils.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

using namespace Sudoku;
using namespace Utilities;
namespace str = std::ranges;

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
      house.houseId = {houseType, idx};
      str::copy(idx2cell.at(idx), house.cells.begin());
      housesByIdx.at(idx) = house;
   }

   return housesByIdx;
}

} // namespace

const House &House::getRow(Index row)
{
   static std::vector<House> housesByRow;
   if (housesByRow.empty())
   {
      housesByRow = getHousesBy(HouseType::Row);
   }
   return housesByRow.at(row);
}

const House &House::getCol(Index col)
{
   static std::vector<House> housesByCol;
   if (housesByCol.empty())
   {
      housesByCol = getHousesBy(HouseType::Col);
   }
   return housesByCol.at(col);
}

const House &House::getBox(Index box)
{
   static std::vector<House> housesByBox;
   if (housesByBox.empty())
   {
      housesByBox = getHousesBy(HouseType::Box);
   }
   return housesByBox.at(box);
}

const House &House::getHouse(HouseId houseId)
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

bool CellInHouse::operator()(const Cell &cell) const
{
   return houseId == HouseId::getFromCellaAndType(cell, houseId.houseType);
}

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

FieldBits House::getFieldsInHouse(HouseId houseId)
{
   return SudokuUtils::getFields(getHouse(houseId).cells);
}
