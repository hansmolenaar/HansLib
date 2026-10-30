#pragma once

#include "Cell.h"
#include "SudokuDefines.h"

#include <span>

namespace Sudoku
{

class CandidatesGrid;

struct LockedPair
{
   Value value;
   Cell cell0;
   Cell cell1;
};

class LockedPairs
{
 public:
   explicit LockedPairs(const CandidatesGrid &);
   std::span<const LockedPair> getPairs(Value) const;
};

} // namespace Sudoku
