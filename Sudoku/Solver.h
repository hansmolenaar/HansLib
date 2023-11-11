#pragma once
#include "SudokuDefines.h"
#include "Potentials.h"
#include "Diagram.h"

#include <array>
#include <span>

namespace Sudoku
{

   class Solver
   {
   public:
      static Solver Create(const Diagram& diagramIn);
      static void Solve(Potentials& potentials);
      bool isSolved() const;
      const Diagram& getState() const;

   private:
      Solver(Diagram diagramIn, Diagram state);
      Diagram m_diagramIn;
      Diagram m_diagramState;
      bool m_isSolved;
   };

}