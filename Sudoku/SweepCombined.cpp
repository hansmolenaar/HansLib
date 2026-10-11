#include "SweepCombined.h"

using namespace Sudoku;

SweepCombined::SweepCombined(std::initializer_list<ISweep *> sweepers) : m_sweepers(sweepers)
{
}

void SweepCombined::sweep(CandidatesGrid &grid)
{
   for (auto *s : m_sweepers)
   {
      s->sweep(grid);
   }
}
