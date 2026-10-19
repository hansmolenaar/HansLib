#pragma once

#include "SudokuDefines.h"
#include <boost/container/static_vector.hpp>

#include <initializer_list>
#include <optional>

namespace Sudoku
{
class Candidates
{
 public:
   void setOnly(std::initializer_list<Value>);
   void removeCandidate(Value);
   bool isSolved() const;
   Index size() const;
   std::optional<Value> tryGetSolvedValue() const;
   bool contains(Value) const;

   static std::array<std::string, NumBoxIndex> ToStringsDifferences(const Candidates &);
   static std::array<std::string, NumBoxIndex> ToStrings(const Candidates &);

 private:
   boost::container::static_vector<Value, NumIndex> m_candidates = {1, 2, 3, 4, 5, 6, 7, 8, 9};
};
} // namespace Sudoku
