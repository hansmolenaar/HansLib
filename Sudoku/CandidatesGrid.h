#pragma once

#include "Candidates.h"
#include "FieldValue.h"

#include <vector>

namespace Sudoku
{
struct Cell;
class FieldsInfo;

class CandidatesGrid
{
 public:
   Index getNumCandidates() const;

   void setGiven(Field, Value);
   void setGiven(Index, Index, Value);
   bool setCandidates(Field, std::initializer_list<Value>);
   bool setCandidates(Index, Index, std::initializer_list<Value>);
   bool removeCandidate(Field, Value);

   void update(FieldsInfo &);     // set candidates from givens
   void checkValidSudoku() const; // throws if not valid
   bool isSolved() const;
   GridChange apply(const FieldValue::Container &);
   GridChange apply(const FieldValue::Container &, FieldsInfo &);
   bool applyAndUpdate(const FieldValue::Container &, FieldsInfo &);

   static CandidatesGrid FromString(const std::string &, char = ValueUndefinedChar);
   static CandidatesGrid FromValues(const std::array<Value, NumField> &);
   static std::vector<std::string> DifferenceToStrings(const CandidatesGrid &, const CandidatesGrid &);
   static std::vector<CandidatesGrid> getSymmetricEquivalents(const CandidatesGrid &);
   static ValueBits getValuesInFields(const CandidatesGrid &, std::span<const Field>);

   const Candidates &operator[](Field f) const
   {
      return m_candidates[f];
   }

 private:
   std::array<Candidates, NumField> m_candidates;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::CandidatesGrid &);
