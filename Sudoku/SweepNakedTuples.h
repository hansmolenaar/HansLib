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
FieldValue::Container collectHouse(const FieldsInfo &, const House &, Index);
}; // namespace SweepNakedTuples

class SweepNakedPairs : public ISweep
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
};

class SweepNakedTriplet : public ISweep
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
};

class SweepNakedQuartet : public ISweep
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
};

} // namespace Sudoku
