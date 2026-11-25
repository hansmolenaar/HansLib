#include "StrategyXWing.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "LockedPairs.h"
#include "Logging.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include "Visibility.h"
#include <optional>

using namespace Sudoku;
using namespace Utilities;

namespace
{
std::string GetName()
{
   return "X-wing";
}

void collectCandidates(const FieldsInfo &finfo, Value value, const XWing &xWing, FieldValues &fieldValues)
{
   FieldBits fixedFields;
   fixedFields.set(xWing.lockedPair0[0].field);
   fixedFields.set(xWing.lockedPair0[1].field);
   fixedFields.set(xWing.lockedPair1[0].field);
   fixedFields.set(xWing.lockedPair1[1].field);
   const auto activeFields = finfo.getHouseFields(xWing.house0) & ~fixedFields & finfo.getActiveFieldsWithValue(value);
   const auto prvSize = fieldValues.size();
   fieldValues.add(activeFields, value);
   const auto change = fieldValues.size() - prvSize;
   if (Logging::getDoLog() && change > 0)
   {
      Logging logMessage;
      logMessage.strategyName = GetName();
      logMessage.numChanges = change;
      logMessage.info << "LockedPair " << xWing.lockedPair0 << " in " << xWing.house0;
      logMessage.info << " and " << xWing.lockedPair1 << " in " << xWing.house1;
      std::cout << logMessage;
   }
}

bool IsSimpleLineLockedPair(const LockedPair &lp)
{
   const auto visibility = Visibility::areVisibleVia(lp[0], lp[1]);
   if (visibility.size() != 1)
   {
      return false;
   }
   return visibility.at(0) != HouseType::Box;
}

std::optional<House> getWeakConnectionVisibility(const Cell &cell0, const Cell &cell1)
{
   if (cell0 == cell1)
   {
      return {};
   }
   const auto visibility = Visibility::areVisibleVia(cell0, cell1);
   if (visibility.size() == 1)
   {
      const auto type = visibility.front();
      if (type != HouseType::Box)
      {
         return {House::getFromCellaAndType(cell0, type)};
      }
   }
   return {};
}

}; // namespace

std::optional<XWing> StrategyXWing::isXWing(std::span<const LockedPair> candidatePair)
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

void StrategyXWing::collect(const FieldsInfo &finfo, FieldValues &result)
{
   const LockedPairs lps(finfo);
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
            collectCandidates(finfo, value, xWing.value(), result);
         }
      } while (combinations.next());
   }
}

std::string StrategyXWing::getName() const
{
   return GetName();
}
