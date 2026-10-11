#pragma once

#include "CandidatesGrid.h"
#include <optional>

namespace Sudoku
{
class Grid
{
 public:
   void update(); // set candidates from solved values
   void setGiven(Index, Index, Value);
   void setGiven(Field, Value);

   Index getNumCandidates() const;
   std::optional<Value> tryGetSolvedValue(Field) const;

   std::optional<Value> tryGetSolvedValue(Index, Index) const;

   bool isSolved() const;
   void checkValidSudoku() const; // throws if not valid

 private:
   CandidatesGrid m_candidates;
};
} // namespace Sudoku

std::ostream &operator<<(std::ostream &os, const Sudoku::Grid &);
