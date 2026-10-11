#pragma once

#include "ISweep.h"

namespace Sudoku
{

class SweepHiddenSingle : public ISweep
{
 public:
   void sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
