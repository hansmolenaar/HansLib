#pragma once

#include "ISweep.h"

namespace Sudoku
{
class Grid;

class SweepNakedSingle : public ISweep
{
 public:
   //~SweepNakedSingle() = default; TODO
   void sweep(Grid &) override;
};

} // namespace Sudoku
