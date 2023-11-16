#pragma once
#include "SudokuDefines.h"
#include "Potentials.h"

namespace Sudoku
{
   enum class SolverSweepResult  {NoChange, Change, Solved};

   class ISolverSweep
   {
   public:
      ~ISolverSweep() noexcept = default;
      virtual SolverSweepResult operator()(Potentials& potentials) = 0;
   };

   class SolverSweepTrivial : public ISolverSweep
   {
   public:
      SolverSweepResult operator()(Potentials& potentials) override;
   };

   class SweepAllClusters : public ISolverSweep
   {
   public:
      SolverSweepResult operator()(Potentials& potentials) override;
   };

}