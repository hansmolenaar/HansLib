#include "Sweep.h"
#include "CandidatesGrid.h"
#include "IStrategy.h"
#include "SweepNakedSingle.h"

using namespace Sudoku;

Sweep::Sweep(std::initializer_list<IStrategy *> strategies) : m_strategies(strategies)
{
}

Sweep::Sweep(std::span<IStrategy *> strategies) : m_strategies(strategies.begin(), strategies.end())
{
}

GridChange Sweep::singleSweep(CandidatesGrid &grid, FieldsInfo &finfo) const
{
   FieldValue::Container fieldsAndValues;

   for (auto *strategy : m_strategies)
   {
      const auto fvs = strategy->collect(finfo);
      FieldValue::add(fvs, fieldsAndValues);
   }

   const auto result = grid.apply(fieldsAndValues, finfo);
   return result;
}

bool Sweep::singleSweepAndUpdate(CandidatesGrid &grid, FieldsInfo &finfo) const
{
   const auto changes = singleSweep(grid, finfo);
   if (changes.numFieldsSolved > 0)
   {
      SweepNakedSingle strategyNakedSingles;
      Sweep sweepSingles({&strategyNakedSingles});
      sweepSingles.iterate(grid, finfo);
   }

   return changes.numValuesChanged > 0;
}

bool Sweep::iterate(CandidatesGrid &grid, FieldsInfo &finfo)
{
   bool anyChange = false;
   GridChange changes;
   do
   {
      changes = singleSweep(grid, finfo);
      anyChange |= changes.numValuesChanged > 0;
   } while (changes.numValuesChanged > 0);
   return anyChange;
}
