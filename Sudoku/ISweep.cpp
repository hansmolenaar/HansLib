#include "ISweep.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"

#include <stdexcept>

using namespace Sudoku;

bool ISweep::iterate(ISweep &sweeper, CandidatesGrid &grid)
{
   bool anyChange = false;
   bool changeInSweep = false;
   do
   {
      changeInSweep = sweeper.sweep(grid);
      anyChange |= changeInSweep;
   } while (changeInSweep);
   return anyChange;
}

FieldValue::Container ISweep::collect(const FieldsInfo &)
{
   throw std::runtime_error("ISweep::collect");
}

bool ISweep::sweep(CandidatesGrid &grid)
{
   FieldsInfo finfo(grid);
   const auto fieldValues = collect(finfo);
   const auto changes = grid.apply(grid, fieldValues);
   return changes.numValuesChanged > 0;
}
