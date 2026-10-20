#pragma once

#include "ISweep.h"
#include "SudokuDefines.h"

namespace Sudoku
{

class SweepHiddenTuple : public ISweep
{
 public:
   static bool sweep(CandidatesGrid &, Index);
   bool sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
