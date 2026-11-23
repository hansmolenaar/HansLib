#pragma once

#include "IStrategy.h"

namespace Sudoku
{

class StrategyNakedSingle : public IStrategy
{
 public:
   bool collect(const FieldsInfo &, FieldValue::Container &) override;
};

} // namespace Sudoku
