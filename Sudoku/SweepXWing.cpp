#include "SweepXWing.h"
#include "Combinations.h"
#include "LockedPairs.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include <optional>

using namespace Sudoku;
using namespace Utilities;

namespace
{
bool RemoveCandidates(CandidatesGrid &grid, const XWing &xWing)
{
   bool anyChange = false;
   return anyChange;
}

}; // namespace

std::optional<XWing> SweepXWing::isXWing(std::span<const LockedPair> candidatePair)
{
   MyAssert(candidatePair.size() == 2);
   return {};
}

bool SweepXWing::sweep(CandidatesGrid &grid)
{
   bool anyChange = false;
   const LockedPairs lps(grid);
   for (Value value : ValueAll)
   {
      const auto lockedPairs = lps.getPairs(value);
      if (lockedPairs.size() < 2)
      {
         continue;
      }
      Combinations<LockedPair> combinations(lockedPairs, 2);
      do
      {
         const auto xWing = isXWing(combinations());
         if (xWing.has_value())
         {
            anyChange |= RemoveCandidates(grid, xWing.value());
         }
      } while (combinations.next());
   }
   return anyChange;
}
