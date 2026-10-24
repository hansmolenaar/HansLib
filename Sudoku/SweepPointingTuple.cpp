
#include "SweepPointingTuple.h"
#include "SudokuUtils.h"
#include <boost/container/static_vector.hpp>

using namespace Sudoku;

bool SweepPointingTuple::sweepBox(CandidatesGrid &grid, const House &box, Index tupleSize)
{
   bool anyChange = false;
   std::array<boost::container::static_vector<Cell, NumCellsInHouse>, NumValue + 1> valueInFields;
   // for (Value value : ValueAll)
   {
   }
   return anyChange;
}
