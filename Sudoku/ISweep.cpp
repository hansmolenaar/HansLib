#include "ISweep.h"
#include "CandidatesGrid.h"

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
