#pragma once

#include "CandidatesGrid.h"

#include <vector>

namespace Sudoku
{

namespace TestModels
{
std::vector<CandidatesGrid> getSimplePuzzles();
std::vector<CandidatesGrid> getMediumPuzzles();
std::vector<CandidatesGrid> getHardPuzzles();
}; // namespace TestModels

} // namespace Sudoku
