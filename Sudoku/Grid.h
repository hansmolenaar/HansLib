#pragma once

#include "Candidates.h"
#include <array>

namespace Sudoku
{
class Grid
{
 public:
   void setGiven(Field, Value);
   Index getNumCandidates() const;

 private:
   std::array<Candidates, NumField> m_grid;
};
} // namespace Sudoku
