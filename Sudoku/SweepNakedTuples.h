#pragma once

#include "House.h"
#include "ISweep.h"

namespace Sudoku
{

class FieldsInfo;

namespace SweepNakedTuples
{
bool sweep(CandidatesGrid &, Index);
bool sweepHouse(CandidatesGrid &grid, const FieldsInfo &, HouseId, Index);
bool sweepHousesOfType(CandidatesGrid &, const FieldsInfo &, HouseType, Index);
}; // namespace SweepNakedTuples

class SweepNakedPairs : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

class SweepNakedTriplet : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

class SweepNakedQuartet : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
};

} // namespace Sudoku
