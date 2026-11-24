#pragma once

#include "IStrategy.h"

namespace Sudoku
{

class FieldsInfo;

class StrategyNakedTuples : public IStrategy
{
 public:
   static void collectOfTupleSize(const FieldsInfo &, Index, FieldValues &);
   static void collectHousesOfType(const FieldsInfo &, HouseType, Index, FieldValues &);
   void collect(const FieldsInfo &, FieldValues &) override;
};

} // namespace Sudoku
