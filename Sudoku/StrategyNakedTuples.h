#pragma once

#include "House.h"
#include "IStrategy.h"

namespace Sudoku
{

class FieldsInfo;

class StrategyNakedTuples : public IStrategy
{
 public:
   static FieldValue::Container collectOfTupleSize(const FieldsInfo &, Index);
   static FieldValue::Container collectHousesOfType(const FieldsInfo &, HouseType, Index);
   static FieldValue::Container collectHouse(const FieldsInfo &, const House &, Index);
   bool collect(const FieldsInfo &, FieldValue::Container &) override;
};

} // namespace Sudoku
