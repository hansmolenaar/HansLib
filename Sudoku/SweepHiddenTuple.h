#pragma once

#include "House.h"
#include "ISweep.h"
#include "SudokuDefines.h"

namespace Sudoku
{

namespace SweepHiddenTuple
{
bool sweepHouse(CandidatesGrid &, const House &, Index);
bool sweep(CandidatesGrid &, Index);
}; // namespace SweepHiddenTuple

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
