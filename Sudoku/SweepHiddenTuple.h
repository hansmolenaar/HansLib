#pragma once

#include "House.h"
#include "ISweep.h"
#include "SudokuDefines.h"

namespace Sudoku
{

class SweepHiddenTuple : public ISweep
{
 public:
   static bool sweepHouse(CandidatesGrid &, const House &, Index);
   static bool sweep(CandidatesGrid &, Index);
   bool sweep(CandidatesGrid &) override;
};
class SweepHiddenSingle : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
