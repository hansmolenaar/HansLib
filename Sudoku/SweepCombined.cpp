#include "SweepCombined.h"
#include "SweepHiddenTuple.h"
#include "SweepNakedSingle.h"

using namespace Sudoku;

namespace
{

SweepNakedSingle sweepNakedSingle;
SweepHiddenSingle sweepHiddenSingle;

} // namespace

SweepCombined::SweepCombined(std::initializer_list<ISweep *> sweepers) : m_sweepers(sweepers)
{
}

SweepCombined::SweepCombined(std::span<ISweep *> sweepers) : m_sweepers(sweepers.begin(), sweepers.end())
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

SweepCombined *SweepCombined::getSimpleSweepers()
{
   static SweepCombined combined{&sweepNakedSingle, &sweepHiddenSingle};
   return &combined;
}
