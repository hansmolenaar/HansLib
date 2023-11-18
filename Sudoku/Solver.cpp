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

bool Solver::Solve(Potentials& potentials)
{
   SolverSweep sweep;
   return sweep(potentials) == SolverSweepResult::Solved;
}

Solver Solver::Create(const Diagram& diagramIn)
{
   Potentials potentials = diagramIn.getPotentials();
   Solve(potentials);
   return Solver(diagramIn, Diagram::Create(potentials));
}

Diagram Solver::Solve(const Diagram& diagram)
{
   auto potentials = diagram.getPotentials();
   SolverSweep sweep;
   sweep(potentials);
   return Diagram::Create(potentials);
}