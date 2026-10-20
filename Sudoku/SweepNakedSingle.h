#pragma once

#include "ISweep.h"

namespace Sudoku
{

class SweepNakedSingle : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
