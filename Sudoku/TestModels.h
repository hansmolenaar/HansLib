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
std::vector<std::string> getHardPuzzlesInStringFormat();

CandidatesGrid getSolvedGrid();

}; // namespace TestModels

} // namespace Sudoku
