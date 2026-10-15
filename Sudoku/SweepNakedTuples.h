#pragma once

#include "House.h"
#include "ISweep.h"

namespace Sudoku
{

class SweepNakedTuples : public ISweep
{
 public:
   void sweep(CandidatesGrid &) override;
   void sweepHouseOfType(CandidatesGrid &, HouseType, std::initializer_list<Index>);

 private:
};

} // namespace Sudoku
