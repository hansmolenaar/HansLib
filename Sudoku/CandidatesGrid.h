#pragma once

#include "Candidates.h"

namespace Sudoku
{
class CandidatesGrid
{
 public:
   Index getNumCandidates() const;

   void setCandidates(Field, std::initializer_list<Value>);
   void removeCandidate(Field, Value);

   const Candidates &operator[](Field f) const
   {
      return m_candidates[f];
   }
   // std::optional<Value> tryGetSolvedValue(Field) const;
   // std::optional<Value> tryGetSolvedValue(Index, Index) const;

 private:
   std::array<Candidates, NumField> m_candidates;
};
} // namespace Sudoku
