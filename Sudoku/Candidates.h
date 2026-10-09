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
   void setGiven(Value);
   void setOnly(std::initializer_list<Value>);
   bool unsetValue(Value);
   bool isSolved() const;
   Index size() const;
   std::optional<Value> tryGetSolvedValue() const;
   bool contains(Value) const;

 private:
   boost::container::static_vector<Value, NumIndex> m_candidates = {1, 2, 3, 4, 5, 6, 7, 8, 9};
};
} // namespace Sudoku
