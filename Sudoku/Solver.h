#pragma once
#include "Diagram.h"
#include "Potentials.h"

namespace Sudoku
{

namespace Solver
{
bool Solve(Potentials &potentials);
Diagram Solve(const Diagram &diagram);
} // namespace Solver

} // namespace Sudoku
