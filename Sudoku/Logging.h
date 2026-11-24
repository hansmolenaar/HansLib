#pragma once

#include "SudokuDefines.h"
#include <string>

namespace Sudoku
{

struct Logging
{
   std::string strategyName;
   std::string info;
   size_t numChanges = 12345;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::Logging &);
std::ostream &operator<<(std::ostream &, const Sudoku::ValueBits &);
std::ostream &operator<<(std::ostream &, std::span<const Sudoku::Field>);
