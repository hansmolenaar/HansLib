#pragma once

#include "Candidates.h"
#include <vector>

namespace Sudoku
{
struct Cell;

class CandidatesGrid
{
 public:
   Index getNumCandidates() const;

   void setGiven(Field, Value);
   void setGiven(Index, Index, Value);
   bool setCandidates(Field, std::initializer_list<Value>);
   bool setCandidates(Index, Index, std::initializer_list<Value>);
   bool removeCandidate(Field, Value);
   void update();                 // set candidates from givens
   void checkValidSudoku() const; // throws if not valid
   bool isSolved() const;

   static CandidatesGrid FromString(const std::string &, char = ValueUndefinedChar);
   static CandidatesGrid FromValues(const std::array<Value, NumField> &);
   static std::vector<std::string> DifferenceToStrings(const CandidatesGrid &, const CandidatesGrid &);
   static std::vector<CandidatesGrid> getSymmetricEquivalents(const CandidatesGrid &);
   static ValueBits getValuesInFields(const CandidatesGrid &, std::span<const Field>);

   const Candidates &operator[](Field f) const
   {
      return m_candidates[f];
   }

   template <typename R> bool RemoveCandidates(std::span<const Value> valuesToRemove, R &fromCells)
   {
      bool anyChange = false;
      for (const auto &cell : fromCells)
      {
         for (Value value : valuesToRemove)
         {
            anyChange |= removeCandidate(cell.field, value);
         }
      }
      return anyChange;
   }

 private:
   std::array<Candidates, NumField> m_candidates;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::CandidatesGrid &);
