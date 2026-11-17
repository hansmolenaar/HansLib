#pragma once

#include "House.h"
#include "ISweep.h"

namespace Sudoku
{

class FieldsInfo;

namespace SweepNakedTuples
{
FieldValue::Container collect(const FieldsInfo &, Index);
FieldValue::Container collectHousesOfType(const FieldsInfo &, HouseType, Index);
FieldValue::Container collectHouse(const FieldsInfo &, HouseId, Index);
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
