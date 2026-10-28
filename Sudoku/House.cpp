#include "House.h"
#include "MyAssert.h"

#include <algorithm>
#include <vector>

using namespace Sudoku;
using namespace Utilities;
namespace str = std::ranges;

namespace
{
template <typename F> std::vector<House> getHousesBy(HouseType houseType, F &&selector)
{
   std::vector<House> housesByIdx(NumIndex);
   std::array<std::vector<Cell>, NumIndex> idx2cell;
   for (Field f : FieldAll)
   {
      const auto &cell = Cell::get(f);
      idx2cell.at(selector(cell)).push_back(cell);
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
      housesByRow = getHousesBy(HouseType::Row, [](const Cell &cell) { return cell.row; });
   }
   return housesByRow.at(row);
}

const House &House::getCol(Index col)
{
   static std::vector<House> housesByCol;
   if (housesByCol.empty())
   {
      housesByCol = getHousesBy(HouseType::Col, [](const Cell &cell) { return cell.col; });
   }
   return housesByCol.at(col);
}

const House &House::getBox(Index box)
{
   static std::vector<House> housesByBox;
   if (housesByBox.empty())
   {
      housesByBox = getHousesBy(HouseType::Box, [](const Cell &cell) { return cell.box; });
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
