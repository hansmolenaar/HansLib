#pragma once

#include "Candidates.h"

namespace Sudoku
{
class CandidatesGrid
{
 public:
   Index getNumCandidates() const;

   void setGiven(Field, Value);
   void setGiven(Index, Index, Value);
   void setCandidates(Field, std::initializer_list<Value>);
   void removeCandidate(Field, Value);
   void update();                 // set candidates from givens
   void checkValidSudoku() const; // throws if not valid
   bool isSolved() const;

   static CandidatesGrid FromString(const std::string &);

   const Candidates &operator[](Field f) const
   {
      return m_candidates[f];
   }

 private:
   std::array<Candidates, NumField> m_candidates;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::CandidatesGrid &);
