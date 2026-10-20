#pragma once

#include "ISweep.h"

namespace Sudoku
{

class SweepHiddenTuple : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
