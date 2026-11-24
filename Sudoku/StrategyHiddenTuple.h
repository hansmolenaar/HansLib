#pragma once

#include "IStrategy.h"
#include "SudokuDefines.h"

namespace Sudoku
{

class StrategyHiddenTuple : public IStrategy
{
 public:
   void collect(const FieldsInfo &, FieldValues &) override;
}; // namespace StrategyHiddenTuple

class StrategyHiddenSingle : public IStrategy
{
 public:
   void collect(const FieldsInfo &, FieldValues &) override;
};

} // namespace Sudoku
