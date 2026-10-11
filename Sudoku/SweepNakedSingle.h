#pragma once

#include "ISweep.h"

namespace Sudoku
{

class SweepNakedSingle : public ISweep
{
 public:
   void sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
