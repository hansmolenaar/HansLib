#pragma once

#include "Cell.h"
#include "SudokuDefines.h"

#include <span>

namespace Sudoku
{

class CandidatesGrid;

class LockedPair
{
 public:
   LockedPair(Value, const Cell &, const Cell &);
   Value getValue() const;
   std::span<const Cell> getCells() const;

   auto operator<=>(const LockedPair &) const = default;

 private:
   Value m_value = ValueUndefined;
   std::array<Cell, 2> m_cells;
};

class LockedPairs
{
 public:
   explicit LockedPairs(const CandidatesGrid &);
   std::span<const LockedPair> getPairs(Value) const;

 private:
   std::array<std::vector<LockedPair>, NumValue + 1> m_lockedPairs;
};

} // namespace Sudoku
