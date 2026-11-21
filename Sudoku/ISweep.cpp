#include "ISweep.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"

using namespace Sudoku;

bool ISweep::iterate(ISweep &sweeper, CandidatesGrid &grid, FieldsInfo &finfo)
{
   bool anyChange = false;
   bool changeInSweep = false;
   do
   {
      changeInSweep = sweeper.sweep(grid);
      anyChange |= changeInSweep;
   } while (changeInSweep);
   if (anyChange)
   {
      finfo.update();
   }
   return anyChange;
}
bool ISweep::sweep(CandidatesGrid &grid)
{
   FieldsInfo finfo(grid);
   const auto fieldValues = collect(finfo);
   const auto changes = grid.apply(fieldValues, finfo);
   return changes.numValuesChanged > 0;
}
