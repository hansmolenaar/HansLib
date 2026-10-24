#pragma once

#include "House.h"
#include "ISweep.h"
#include "SudokuDefines.h"

namespace Sudoku
{

class SweepPointingTuple : public ISweep
{
 public:
   static bool sweepBox(CandidatesGrid &, const House &, Index);
   static bool sweep(CandidatesGrid &, Index);
   bool sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
