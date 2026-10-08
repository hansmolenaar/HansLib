#pragma once

#include "Candidates.h"
#include <array>

namespace Sudoku
{
class Grid
{
 public:
   void setGiven(Field, Value);

 private:
   std::array<Candidates, NumFields> m_grid;
};
} // namespace Sudoku
