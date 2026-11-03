#include "SweepCombined.h"
#include "SweepHiddenTuple.h"
#include "SweepIntersectionElimination.h"
#include "SweepNakedSingle.h"
#include "SweepNakedTuples.h"
#include "SweepXWing.h"

using namespace Sudoku;

namespace
{

SweepNakedSingle sweepNakedSingle;
SweepHiddenSingle sweepHiddenSingle;
SweepNakedPairs sweepNakedPair;
SweepHiddenPair sweepHiddenPair;
SweepNakedTriplet sweepNakedTriplet;
SweepHiddenTriplet sweepHiddenTriplet;
SweepNakedQuartet sweepNakedQuartet;
SweepHiddenQuartet sweepHiddenQuartet;
SweepIntersectionElimination sweepIntersectionElimination;
SweepXWing sweepXWing;

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

SweepCombined *SweepCombined::getMediumSweepers()
{
   static SweepCombined combined{&sweepNakedSingle,  &sweepHiddenSingle,  &sweepNakedPair,
                                 &sweepHiddenPair,   &sweepNakedTriplet,  &sweepHiddenTriplet,
                                 &sweepNakedQuartet, &sweepHiddenQuartet, &sweepIntersectionElimination};
   return &combined;
}

SweepCombined *SweepCombined::getHardSweepers()
{
   static SweepCombined combined{// Simple
                                 &sweepNakedSingle, &sweepHiddenSingle,
                                 // Medium
                                 &sweepNakedPair, &sweepHiddenPair, &sweepNakedTriplet, &sweepHiddenTriplet,
                                 &sweepNakedQuartet, &sweepHiddenQuartet, &sweepIntersectionElimination,
                                 // Hard
                                 &sweepXWing};
   return &combined;
}
