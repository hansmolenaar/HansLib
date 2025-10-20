#pragma once
#include "Diagram.h"
#include "Potentials.h"
#include "SudokuDefines.h"

#include <array>
#include <span>

namespace Sudoku
{

namespace Solver
{
bool Solve(Potentials &potentials);
Diagram Solve(const Diagram &diagram);
} // namespace Solver

} // namespace Sudoku