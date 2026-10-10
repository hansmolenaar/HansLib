#pragma once

#include "ISweep.h"

namespace Sudoku
{
class Grid;

class SweepHiddenSingle : public ISweep
{
 public:
   void sweep(Grid &) override;
};

} // namespace Sudoku
