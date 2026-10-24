#pragma once

#include "House.h"
#include "ISweep.h"

namespace Sudoku
{

class SweepNakedTuples : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;

   static bool sweepHouse(CandidatesGrid &grid, const House &, Index);
   static bool sweepHousesOfType(CandidatesGrid &grid, HouseType, Index);
};

class SweepNakedPairs : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

class SweepNakedTriplet : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

class SweepNakedQuartet : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
