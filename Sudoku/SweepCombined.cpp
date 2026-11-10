#include "SweepCombined.h"
#include "SweepHiddenTuple.h"
#include "SweepIntersectionElimination.h"
#include "SweepNakedSingle.h"
#include "SweepNakedTuples.h"
#include "SweepSimpleColoring.h"
#include "SweepXWing.h"

using namespace Sudoku;

namespace
{

template <typename T, std::size_t N, std::size_t M>
auto concat_arrays(const std::array<T, N> &a, const std::array<T, M> &b)
{
   // assumes `T` is default-constructible
   std::array<T, N + M> r;
   std::copy(std::begin(a), std::end(a), std::begin(r));
   std::copy(std::begin(b), std::end(b), std::begin(r) + N);
   return r;
}

constexpr size_t numSimpleSweepers = 2;
constexpr size_t numMediumSweepers = 7;
constexpr size_t numHardSweepers = 2;

std::array<ISweep *, numSimpleSweepers> getSimple()
{
   static SweepNakedSingle sweepNakedSingle;
   static SweepHiddenSingle sweepHiddenSingle;
   static std::array<ISweep *, numSimpleSweepers> simple{&sweepNakedSingle, &sweepHiddenSingle};
   return simple;
}

std::array<ISweep *, numSimpleSweepers + numMediumSweepers> getMedium()
{
   static SweepNakedPairs sweepNakedPair;
   static SweepHiddenPair sweepHiddenPair;
   static SweepNakedTriplet sweepNakedTriplet;
   static SweepHiddenTriplet sweepHiddenTriplet;
   static SweepNakedQuartet sweepNakedQuartet;
   static SweepHiddenQuartet sweepHiddenQuartet;
   static SweepIntersectionElimination sweepIntersectionElimination;
   static std::array<ISweep *, 7> mediumOnly{
       &sweepNakedPair,
       &sweepHiddenPair,
       &sweepNakedTriplet,
       &sweepHiddenTriplet,
       &sweepNakedQuartet,
       &sweepHiddenQuartet,
       &sweepIntersectionElimination,
   };
   static std::array<ISweep *, numSimpleSweepers + numMediumSweepers> medium =
       concat_arrays<ISweep *, numSimpleSweepers, numMediumSweepers>(getSimple(), mediumOnly);
   return medium;
}

std::array<ISweep *, numSimpleSweepers + numMediumSweepers + numHardSweepers> getHard()
{
   static SweepXWing sweepXWing;
   static SweepSimpleColoring sweepSimpleColoring;
   static std::array<ISweep *, numHardSweepers> hardOnly{&sweepXWing, &sweepSimpleColoring};
   static std::array<ISweep *, numSimpleSweepers + numMediumSweepers + numHardSweepers> hard =
       concat_arrays<ISweep *, numSimpleSweepers + numMediumSweepers, numHardSweepers>(getMedium(), hardOnly);
   return hard;
}

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
   static auto s_sweepers = getSimple();
   static SweepCombined combined(s_sweepers);
   return &combined;
}

SweepCombined *SweepCombined::getMediumSweepers()
{
   static auto s_sweepers = getMedium();
   static SweepCombined combined(s_sweepers);
   return &combined;
}

SweepCombined *SweepCombined::getHardSweepers()
{
   static auto s_sweepers = getHard();
   static SweepCombined combined(s_sweepers);
   return &combined;
}
