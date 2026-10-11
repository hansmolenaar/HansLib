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

#if false
   void setOnly(std::initializer_list<Value>);
   void unsetValue(Value);
   bool isSolved() const;
   Index size() const;
   std::optional<Value> tryGetSolvedValue() const;
   bool contains(Value) const;
#endif

 private:
   std::array<Candidates, NumField> m_candidates;
};
} // namespace Sudoku
