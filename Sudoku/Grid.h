#pragma once

#include "CandidatesGrid.h"
#include <optional>

namespace Sudoku
{
class Grid
{
 public:
   void update(); // set candidates from givens
   void setValue(Index, Index, Value);
   void setValue(Field, Value); // TODO rename setGiven
   void unsetCandidate(Field, Value);
   Index getNumCandidates() const;
   std::optional<Value> tryGetSolvedValue(Field) const;
   std::optional<Value> tryGetSolvedValue(Index, Index) const;
   bool isSolved() const;
   void checkValidSudoku() const; // throws if not valid
   bool isCandidateInCell(Field, Value) const;

   // TODO Remove me
   CandidatesGrid &get()
   {
      return m_candidates;
   }
   const CandidatesGrid &get() const
   {
      return m_candidates;
   }

 private:
   CandidatesGrid m_candidates;
};
} // namespace Sudoku

std::ostream &operator<<(std::ostream &os, const Sudoku::Grid &);
