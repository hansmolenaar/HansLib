#include "SweepXWing.h"
#include "Candidates.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "Contains.h"
#include "LockedPairs.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include "Visibility.h"
#include <algorithm>
#include <optional>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{
bool RemoveCandidates(CandidatesGrid &grid, const XWing &xWing)
{
   std::array<Cell, 4> fixedCells{xWing.lockedPair0[0], xWing.lockedPair0[1], xWing.lockedPair1[0],
                                  xWing.lockedPair1[1]};
   std::array<Value, 2> valuesToRemove;
   const Candidates &c = grid[fixedCells[0].field];
   str::copy(c, valuesToRemove.begin());
   IsNotContained<Cell> isNotFixed{fixedCells};
   const bool anyChange = grid.RemoveCandidates(valuesToRemove, House::getHouse(xWing.houseId0).cells, isNotFixed);
   return anyChange;
}

bool IsSimpleLineLockedPair(const LockedPair &lp)
{
   const auto visibility = Visiblity::areVisibleVia(lp[0], lp[1]);
   if (visibility.size() != 1)
   {
      return false;
   }
   return visibility.at(0) != HouseType::Box;
}

std::optional<HouseId> getWeakConnectionVisibility(const Cell &cell0, const Cell &cell1)
{
   MyAssert(cell0 != cell1);
   const auto visibility = Visiblity::areVisibleVia(cell0, cell1);
   if (visibility.size() == 1)
   {
      const auto type = visibility.front();
      if (type != HouseType::Box)
      {
         return {HouseId::getFromCellaAndType(cell0, type)};
      }
   }
   return {};
}

}; // namespace

std::optional<XWing> SweepXWing::isXWing(std::span<const LockedPair> candidatePair)
{
   MyAssert(candidatePair.size() == 2);
   const auto &lp0 = candidatePair[0];
   const auto &lp1 = candidatePair[1];
   if (!IsSimpleLineLockedPair(lp0) || !IsSimpleLineLockedPair(lp1))
   {
      return {};
   };

   auto weakConn0 = getWeakConnectionVisibility(lp0[0], lp1[0]);
   auto weakConn1 = getWeakConnectionVisibility(lp0[1], lp1[1]);
   if (weakConn0.has_value() && weakConn1.has_value())
   {
      return XWing{lp0, *weakConn0, lp1, *weakConn1};
   }

   weakConn0 = getWeakConnectionVisibility(lp0[0], lp1[1]);
   weakConn1 = getWeakConnectionVisibility(lp0[1], lp1[0]);
   if (weakConn0.has_value() && weakConn1.has_value())
   {
      return XWing{lp0, *weakConn0, lp1, *weakConn1};
   }
   return {};
}

bool SweepXWing::sweep(CandidatesGrid &grid)
{
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
            const bool anyChange = RemoveCandidates(grid, xWing.value());
            if (anyChange)
            {
               return true;
            }
         }
      } while (combinations.next());
   }
   return false;
}
