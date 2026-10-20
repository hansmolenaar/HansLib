#pragma once

#include "ISweep.h"

namespace Sudoku
{

class SweepHiddenSingle : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
