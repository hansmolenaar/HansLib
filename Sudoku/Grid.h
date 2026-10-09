#pragma once

#include "Candidates.h"
#include <array>
#include <optional>

namespace Sudoku
{
class Grid
{
 public:
   void setGiven(Field, Value);
   void setGiven(Index, Index, Value);
   Index getNumCandidates() const;
   std::optional<Value> tryGetSolvedValue(Field) const;
   bool isSolved() const;

 private:
   std::array<Candidates, NumField> m_candidates;
};
} // namespace Sudoku

std::ostream &operator<<(std::ostream &os, const Sudoku::Grid &);
