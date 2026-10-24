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
std::vector<Grid> getHardPuzzles();
}; // namespace TestModels

} // namespace Sudoku
