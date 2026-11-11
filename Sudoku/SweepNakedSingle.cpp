#include "SweepNakedSingle.h"
#include "CandidatesGrid.h"
#include "Functors.h"
#include "Visibility.h"

using namespace Sudoku;

bool SweepNakedSingle::sweep(CandidatesGrid &grid)
{
   bool anyChange = false;
   for (Field field : FieldAll)
   {
      const auto value = grid[field].tryGetSolvedValue();
      if (value)
      {
         const auto cells = Visiblity::getVisibleCells(field);
         anyChange |= grid.RemoveCandidate(*value, cells);
      }
   }
   return anyChange;
}
