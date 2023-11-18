#include "Potential.h"
#include "Defines.h"
#include "FieldInfoStatic.h"
#include "Solver.h"
#include "SolverSweep.h"

using namespace Sudoku;

bool Solver::Solve(Potentials& potentials)
{
   SolverSweep sweep;
   return sweep(potentials) == SolverSweepResult::Solved;
}

Diagram Solver::Solve(const Diagram& diagram)
{
   auto potentials = diagram.getPotentials();
   SolverSweep sweep;
   sweep(potentials);
   return Diagram::Create(potentials);
}