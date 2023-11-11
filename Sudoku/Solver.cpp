#include "Potential.h"
#include "Defines.h"
#include "FieldInfoStatic.h"
#include "Solver.h"
#include "SolverSweep.h"

using namespace Sudoku;

Solver::Solver(Diagram diagramIn, Diagram state) :
   m_diagramIn(std::move(diagramIn)),
   m_diagramState(std::move(state)),
   m_isSolved(m_diagramState.isSolved())
{
}

const Diagram& Solver::getState() const
{
   return m_diagramState;
}

bool Solver::isSolved() const
{
   return m_isSolved;
}

Solver Solver::Create(const Diagram& diagramIn)
{
   Potentials potentials = diagramIn.getPotentials();
   bool changed = false;
   do
   {
      do
      {
         changed = SolverSweepTrivial()(potentials);
      } while (changed);

      if (SolverSweepPairs()(potentials))
      {
         changed = true;
      }
   } while (changed);
   return Solver(diagramIn, Diagram::Create(potentials));
}