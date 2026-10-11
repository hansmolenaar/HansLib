#include "ISweep.h"
#include "CandidatesGrid.h"

using namespace Sudoku;

void ISweep::iterate(ISweep &sweeper, CandidatesGrid &grid)
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
