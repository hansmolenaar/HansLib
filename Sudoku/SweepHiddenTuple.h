#pragma once

#include "House.h"
#include "ISweep.h"
#include "SudokuDefines.h"

namespace Sudoku
{

class SweepHiddenTuple
{
 public:
   static bool sweepHouse(CandidatesGrid &, const House &, Index);
   static bool sweep(CandidatesGrid &, Index);
};

class SweepHiddenSingle : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

class SweepHiddenPair : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

class SweepHiddenTriplet : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

class SweepHiddenQuartet : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
