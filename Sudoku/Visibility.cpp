#include "Visibility.h"
#include "BitsetUtils.h"
#include "Functors.h"
#include "House.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"

#include <algorithm>
#include <iterator>
#include <ranges>
#include <vector>

using namespace Sudoku;
using namespace Utilities;

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
