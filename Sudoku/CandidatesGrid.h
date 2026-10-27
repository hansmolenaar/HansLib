#pragma once

#include "Candidates.h"
#include <vector>

namespace Sudoku
{
class CandidatesGrid
{
 public:
   Index getNumCandidates() const;

   void setGiven(Field, Value);
   void setGiven(Index, Index, Value);
   bool setCandidates(Field, std::initializer_list<Value>);
   bool removeCandidate(Field, Value);
   void update();                 // set candidates from givens
   void checkValidSudoku() const; // throws if not valid
   bool isSolved() const;

   static CandidatesGrid FromString(const std::string &);
   static CandidatesGrid FromValues(const std::array<Value, NumField> &);
   static std::vector<std::string> DifferenceToStrings(const CandidatesGrid &, const CandidatesGrid &);

   const Candidates &operator[](Field f) const
   {
      return m_candidates[f];
   }

   template <typename C, typename P>
   bool RemoveCandidates(std::span<const Value> valuesToRemove, const C &allFields, P useFieldPredicate)
   {
      bool anyChange = false;
      for (Field field : allFields)
      {
         if (useFieldPredicate(field))
         {
            for (Value value : valuesToRemove)
            {
               anyChange |= removeCandidate(field, value);
            }
         }
      }
      return anyChange;
   }

 private:
   std::array<Candidates, NumField> m_candidates;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::CandidatesGrid &);
