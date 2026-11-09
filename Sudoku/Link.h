#pragma once

#include "Cell.h"

namespace Sudoku
{

// Ordered pair of cells? cell1 < cell2
struct Link
{
   Link() = default;
   Link(const Cell &, const Cell &, LinkType);
   Cell cellMin;
   Cell cellMax;
   LinkType type;
};

} // namespace Sudoku
