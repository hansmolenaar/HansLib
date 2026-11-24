#pragma once

#include "IStrategy.h"

namespace Sudoku
{

class StrategyNakedSingle : public IStrategy
{
 public:
   void collect(const FieldsInfo &, FieldValues &) override;
};

} // namespace Sudoku
