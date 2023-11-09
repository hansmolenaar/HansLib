#pragma once
#include "SudokuDefines.h"
#include "Potentials.h"

namespace Sudoku
{
   class ISolverSweep
   {
   public:
      ~ISolverSweep() noexcept = default;
      // Result indicates any changes
      virtual bool operator()(Potentials& potentials) = 0;
   };

   class SolverSweepRow : public ISolverSweep
   {
   public:
      bool operator()(Potentials& potentials) override;
   };
}