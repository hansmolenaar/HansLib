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

bool Solver::Solve(Potentials& potentials)
{
   SolverSweepClusters sweepClusters;
   SolverSweepTrivial sweepTrivial;

   SolverSweepResult sweepResult = SolverSweepResult::NoChange;
   do
   {
      sweepResult = SolverSweepIterate(sweepTrivial)(potentials);


      if (sweepClusters(potentials) == SolverSweepResult::Change)
      {
         sweepResult = SolverSweepResult::Change;
      }
   } while (sweepResult == SolverSweepResult::Change);
   return potentials.isSolved();
}

Solver Solver::Create(const Diagram& diagramIn)
{
   Potentials potentials = diagramIn.getPotentials();
   Solve(potentials);
   return Solver(diagramIn, Diagram::Create(potentials));
}