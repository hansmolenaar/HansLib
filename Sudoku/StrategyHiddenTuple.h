#pragma once

#include "IStrategy.h"

namespace Sudoku
{

class StrategyHiddenTuple : public IStrategy
{
 public:
   bool collect(const FieldsInfo &, FieldValue::Container &) override;
}; // namespace StrategyHiddenTuple

class StrategyHiddenSingle : public IStrategy
{
 public:
   bool collect(const FieldsInfo &, FieldValue::Container &) override;
};

} // namespace Sudoku
