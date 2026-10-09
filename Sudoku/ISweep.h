#pragma once

namespace Sudoku
{
class Grid;

class ISweep
{
 public:
   virtual ~ISweep() = default;
   virtual void sweep(Grid &) = 0;
};

} // namespace Sudoku
