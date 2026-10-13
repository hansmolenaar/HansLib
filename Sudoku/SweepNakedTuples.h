#pragma once

#include "ISweep.h"

namespace Sudoku
{

class SweepNakedTuples : public ISweep
{
 public:
   // SweepNakedTuples();
   void sweep(CandidatesGrid &) override;

 private:
};

} // namespace Sudoku
