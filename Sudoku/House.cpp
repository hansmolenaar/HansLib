#include "House.h"
#include "MyAssert.h"

#include <algorithm>
#include <vector>

using namespace Sudoku;
using namespace Utilities;
namespace str = std::ranges;

namespace
{
template <typename F> std::vector<House> getHousesBy(F &&selector)
{
   std::vector<House> housesByIdx(NumIndex);
   std::array<std::vector<Cell>, NumIndex> idx2cell;
   for (Field f = 0; f < NumField; ++f)
   {
      const auto &cell = Cell::get(f);
      idx2cell.at(selector(cell)).push_back(cell);
   }

   for (auto idx : IndexAll)
   {
      MyAssert(idx2cell.at(idx).size() == NumIndex);
      House house;
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
      housesByRow = getHousesBy([](const Cell &cell) { return cell.row; });
   }
   return housesByRow.at(row);
}

const House &House::getCol(Index col)
{
   static std::vector<House> housesByCol;
   if (housesByCol.empty())
   {
      housesByCol = getHousesBy([](const Cell &cell) { return cell.col; });
   }
   return housesByCol.at(col);
}

const House &House::getBox(Index box)
{
   static std::vector<House> housesByBox;
   if (housesByBox.empty())
   {
      housesByBox = getHousesBy([](const Cell &cell) { return cell.box; });
   }
   return housesByBox.at(box);
}
