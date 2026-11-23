#pragma once

#include "House.h"
#include "IStrategy.h"
#include "SudokuDefines.h"

namespace Sudoku
{

class StrategyHiddenTuple : public IStrategy
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;

   static FieldValue::Container collectHouse(const FieldsInfo &, const House &, Index);
   static FieldValue::Container collectTuplesOfSize(const FieldsInfo &, Index);
   // TODO static FieldValue::Container collect(const FieldsInfo &);

}; // namespace StrategyHiddenTuple

class StrategyHiddenSingle : public IStrategy
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
};

} // namespace Sudoku
