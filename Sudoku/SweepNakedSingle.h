#pragma once

#include "IStrategy.h"

namespace Sudoku
{

class SweepNakedSingle : public IStrategy
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
};

} // namespace Sudoku
