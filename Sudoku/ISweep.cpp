#include "ISweep.h"
#include "Grid.h"

using namespace Sudoku;

void ISweep::iterate(ISweep &sweeper, Grid &grid)
{
   auto nCandidatesPrv = grid.getNumCandidates();
   auto nCandidatesCur = nCandidatesPrv;
   do
   {
      sweeper.sweep(grid);
      nCandidatesPrv = nCandidatesCur;
      nCandidatesCur = grid.getNumCandidates();
   } while (nCandidatesCur < nCandidatesPrv);
}
