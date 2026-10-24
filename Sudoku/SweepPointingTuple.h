#pragma once

#include "House.h"
#include "ISweep.h"
#include "SudokuDefines.h"

namespace Sudoku
{

class SweepPointingTuple
{
 public:
   static bool sweepBox(CandidatesGrid &, const House &, Index);
   static bool sweep(CandidatesGrid &, Index);
};

class SweepPointingPair : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

class SweepPointingTriplet : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
