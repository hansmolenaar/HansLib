#pragma once

#include "House.h"
#include "ISweep.h"

namespace Sudoku
{

class FieldsInfo;

class SweepNakedTuples : public ISweep
{
 public:
   static FieldValue::Container collectOfTupleSize(const FieldsInfo &, Index);
   static FieldValue::Container collectHousesOfType(const FieldsInfo &, HouseType, Index);
   static FieldValue::Container collectHouse(const FieldsInfo &, const House &, Index);
   FieldValue::Container collect(const FieldsInfo &) override;
};

} // namespace Sudoku
