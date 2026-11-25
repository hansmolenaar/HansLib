#pragma once

#include "IStrategy.h"

namespace Sudoku
{

class FieldsInfo;

class StrategyNakedTuple : public IStrategy
{
 public:
   void collect(const FieldsInfo &, FieldValues &) override;
   std::string getName() const override;

   static void collectOfTupleSize(const FieldsInfo &, Index, FieldValues &);
   static void collectHousesOfType(const FieldsInfo &, HouseType, Index, FieldValues &);
};

} // namespace Sudoku
