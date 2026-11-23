#pragma once

#include "House.h"
#include "IStrategy.h"
#include "SudokuDefines.h"

namespace Sudoku
{

class StrategyHiddenTuple : public IStrategy
{
 public:
   bool collect(const FieldsInfo &, FieldValue::Container &) override;

   static void collectHouse(const FieldsInfo &, const House &, Index, FieldValue::Container &);
   static void collectTuplesOfSize(const FieldsInfo &, Index, FieldValue::Container &);
   // TODO static FieldValue::Container collect(const FieldsInfo &);

}; // namespace StrategyHiddenTuple

class StrategyHiddenSingle : public IStrategy
{
 public:
   bool collect(const FieldsInfo &, FieldValue::Container &) override;
};

} // namespace Sudoku
