#pragma once

#include "House.h"
#include "ISweep.h"

namespace Sudoku
{

class SweepNakedTuples : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
   bool sweepHouseOfType(CandidatesGrid &, HouseType, std::initializer_list<Index>);
   bool sweepHouseOfType(CandidatesGrid &, HouseType, Index);
   bool sweepHouse(CandidatesGrid &grid, const House &, Index);
   bool sweepHousesOfType(CandidatesGrid &grid, HouseType, Index);
};

} // namespace Sudoku
