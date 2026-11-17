#include "Visibility.h"
#include "SudokuDefines.h"

using namespace Sudoku;

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
