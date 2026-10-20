#include "SweepCombined.h"

using namespace Sudoku;

SweepCombined::SweepCombined(std::initializer_list<ISweep *> sweepers) : m_sweepers(sweepers)
{
}

bool SweepCombined::sweep(CandidatesGrid &grid)
{
   bool anyChange = false;
   for (auto *s : m_sweepers)
   {
      anyChange = s->sweep(grid);
      // Stop after first change, and iterate
      if (anyChange)
      {
         break;
      }
   }
   return anyChange;
}
