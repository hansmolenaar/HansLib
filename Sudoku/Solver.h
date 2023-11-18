#pragma once
#include "SudokuDefines.h"
#include "Potentials.h"
#include "Diagram.h"

#include <array>
#include <span>

namespace Sudoku
{

   namespace Solver
   {
      bool Solve(Potentials& potentials);
      Diagram Solve(const Diagram& diagram);
   }

}