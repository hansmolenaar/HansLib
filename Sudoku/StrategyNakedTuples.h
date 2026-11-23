#pragma once

#include "House.h"
#include "IStrategy.h"

namespace Sudoku
{

class FieldsInfo;

class StrategyNakedTuples : public IStrategy
{
 public:
   static void collectOfTupleSize(const FieldsInfo &, Index, FieldValue::Container &);
   static void collectHousesOfType(const FieldsInfo &, HouseType, Index, FieldValue::Container &);
   static void collectHouse(const FieldsInfo &, const House &, Index, FieldValue::Container &);
   bool collect(const FieldsInfo &, FieldValue::Container &) override;
};

} // namespace Sudoku
