#include "Visibility.h"
#include "Functors.h"
#include "House.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include "UniqueSorted.h"

#include <algorithm>
#include <iterator>
#include <vector>

using namespace Sudoku;
using namespace Utilities;

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
         std::vector<Cell> work(3 * (NumCellsInHouse - 1));
         const Cell &cell = Cell::get(field);
         const Functors::IsNotEqualTo<Cell> isNotEqualTo{cell};
         std::array<HouseId, 3> visibleHouses = {HouseId{HouseType::Row, cell.row}, HouseId{HouseType::Col, cell.col},
                                                 HouseId{HouseType::Box, cell.box}};
         auto itr = work.begin();
         for (auto houseId : visibleHouses)
         {
            str::copy_if(House::getHouse(houseId).cells, itr, isNotEqualTo);
            itr += (NumCellsInHouse - 1);
         }
         UniqueSorted(work);
         MyAssert(work.size() == NumVisibleCellls);
         str::copy(work, visibleCells[field].begin());
      }
      isSet = true;
   }

   return visibleCells[cell.field];
}

std::span<const Cell> Visiblity::getVisibleCells(Field field)
{
   return getVisibleCells(Cell::get(field));
}

boost::container::static_vector<HouseType, 2> Visiblity::areVisibleVia(const Cell &cell0, const Cell &cell1)
{
   if (cell0.field == cell1.field)
      return {};
   boost::container::static_vector<HouseType, 2> result;
   for (HouseType type : HouseTypeAll)
   {
      if (cell0[type] == cell1[type])
      {
         result.push_back(type);
      }
   }
   return result;
}

std::vector<Cell> Visiblity::getVisibleCells(std::span<const Cell> cells)
{
   std::vector<Cell> result;
   for (const auto &cell : cells)
   {
      str::copy(getVisibleCells(cell), std::back_inserter(result));
   }
   UniqueSorted(result);
   return result;
}
