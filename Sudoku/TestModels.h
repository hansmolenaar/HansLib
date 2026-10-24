#pragma once

#include "Grid.h"

#include <vector>

namespace Sudoku
{

namespace TestModels
{
Grid fromString(const std::string &);

std::vector<Grid> getSimplePuzzles();
std::vector<Grid> getMediumPuzzles();
}; // namespace TestModels

} // namespace Sudoku
