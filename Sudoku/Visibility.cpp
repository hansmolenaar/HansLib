#include "Visibility.h"
#include "Functors.h"
#include "House.h"
#include <algorithm>

using namespace Sudoku;

namespace str = std::ranges;

bool Visiblity::areVisible(const Cell &cell0, const Cell &cell1)
{
   if (cell0.field == cell1.field)
   {
      return false;
   }
   return (cell0.row == cell1.row || cell0.col == cell1.col || cell0.box == cell1.box);
} // namespace std::ranges

bool Visiblity::areVisible(Field field0, Field field1)
{
   return areVisible(Cell::get(field0), Cell::get(field1));
}

std::span<const Cell> Visiblity::getVisibleCells(const Cell &cell)
{
   static bool isSet = false;
   static std::array<std::array<Cell, NumVisibleCellls>, NumField> visibleCells;
   if (!isSet)
   {
      for (auto field : FieldAll)
      {
         const Cell &cell = Cell::get(field);
         Functors::IsNotEqualTo<Cell> notEqualTo{cell};
         std::array<HouseId, 3> visibleHouses = {HouseId{HouseType::Row, cell.row}, HouseId{HouseType::Col, cell.col},
                                                 HouseId{HouseType::Box, cell.box}};
         auto itr = visibleCells[field].begin();
         for (auto houseId : visibleHouses)
         {
            str::copy_if(House::getHouse(houseId).cells, itr, notEqualTo);
            itr += 8;
         }
         str::sort(visibleCells[field]);
      }
      isSet = true;
   }

   return visibleCells[cell.field];
}
