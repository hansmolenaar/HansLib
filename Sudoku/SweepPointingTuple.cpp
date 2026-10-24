
#include "SweepPointingTuple.h"
#include "SudokuUtils.h"

using namespace Sudoku;

bool SweepPointingTuple::sweepBox(CandidatesGrid &grid, const House &box, Index tupleSize)
{
   bool anyChange = false;

   const auto activeUniqueCandidates = SudokuUtils::getUniqueActiveValuesInCells(grid, box.cells);
   return anyChange;
}
