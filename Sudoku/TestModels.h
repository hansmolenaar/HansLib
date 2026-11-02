#pragma once

#include "Grid.h"

#include <vector>

namespace Sudoku
{

namespace TestModels
{
std::vector<Grid> getSimplePuzzles();
std::vector<Grid> getMediumPuzzles();
std::vector<Grid> getHardPuzzles();
}; // namespace TestModels

} // namespace Sudoku
