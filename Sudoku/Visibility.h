#pragma once

#include "Cell.h"
#include "SudokuDefines.h"
#include "boost/container/static_vector.hpp"

namespace Sudoku
{

constexpr Field NumVisibleCellls = 20; // NOTE: a cell is NOT visible to itself

struct Visibility
{
   static bool areVisible(const Cell &, const Cell &);
   static bool areVisible(Field, Field);
   static boost::container::static_vector<HouseType, 2> areVisibleVia(const Cell &, const Cell &);
};

} // namespace Sudoku
