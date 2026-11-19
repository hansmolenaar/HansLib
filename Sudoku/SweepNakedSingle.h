#pragma once

#include "ISweep.h"

namespace Sudoku
{

class SweepNakedSingle : public ISweep
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
};

} // namespace Sudoku
