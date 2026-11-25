#pragma once

#include "SudokuDefines.h"
#include <iostream>
#include <sstream>
#include <string>

namespace Sudoku
{

struct Logging
{
   std::string strategyName;
   std::stringstream info;
   size_t numChanges = 12345;

   static void setDoLog(bool);
   static bool getDoLog();
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::Logging &);
std::ostream &operator<<(std::ostream &, const Sudoku::ValueBits &);
std::ostream &operator<<(std::ostream &, const Sudoku::FieldBits &);
std::ostream &operator<<(std::ostream &, std::span<const Sudoku::Field>);
