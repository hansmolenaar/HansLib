#pragma once

#include "SudokuDefines.h"
#include <boost/container/static_vector.hpp>

#include <initializer_list>

namespace Sudoku
{
class Candidates
{
 public:
   void setGiven(Value);
   void setOnly(std::initializer_list<Value>);
   bool unsetValue(Value);
   bool isSolved() const;
   RowColBox size() const;
   Value getSolvedValue() const;
   bool contains(Value) const;

 private:
   boost::container::static_vector<Value, NumRowColBox> m_candidates = {1, 2, 3, 4, 5, 6, 7, 8, 9};
};
} // namespace Sudoku
