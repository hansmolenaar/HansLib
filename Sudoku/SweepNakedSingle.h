#pragma once

#include "ISweep.h"

namespace Sudoku
{

class SweepNakedSingle : public ISweep
{
 public:
   std::set<FieldValue> collect(const FieldsInfo &) override;
};

} // namespace Sudoku
