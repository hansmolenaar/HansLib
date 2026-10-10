#pragma once

#include "Candidates.h"
#include <array>
#include <optional>

namespace Sudoku
{
class Grid
{
 public:
   void update(); // set candidates from givens
   void setGiven(Field, Value);
   void setGiven(Index, Index, Value);
   void setValue(Field, Value); // TODO remove setGiven
   void unsetCandidate(Field, Value);
   Index getNumCandidates() const;
   std::optional<Value> tryGetSolvedValue(Field) const;
   std::optional<Value> tryGetSolvedValue(Index, Index) const;
   bool isSolved() const;
   void checkValidSudoku() const; // throws if not valid
   bool isCandidateInCell(Field, Value) const;

 private:
   std::array<Candidates, NumField> m_candidates;
};
} // namespace Sudoku

std::ostream &operator<<(std::ostream &os, const Sudoku::Grid &);
