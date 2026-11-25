#include "StrategyNakedSingle.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "Logging.h"
#include "SudokuUtils.h"

using namespace Sudoku;
using namespace Utilities;

void StrategyNakedSingle::collect(const FieldsInfo &finfo, FieldValues &result)
{
   const auto &solvedFields = finfo.getSolvedFields();

   const CandidatesGrid &grid = finfo.getGrid();
   for (Field field : SudokuUtils::FieldBitsToRange(solvedFields))
   {
      const Value value = grid[field].tryGetSolvedValue().value();
      auto visibleFields = finfo.getVisibleFields(field) & finfo.getActiveFieldsWithValue(value);
      const auto prvSize = result.size();
      result.add(visibleFields, value);
      if (Logging::getDoLog() && prvSize != result.size())
      {
         Logging logMessage;
         logMessage.strategyName = getName();
         logMessage.numChanges = result.size() - prvSize;
         logMessage.info << "Cell " << Cell::get(field);
         logMessage.info << " with value " << value;
         std::cout << logMessage;
      }
   }
}

std::string StrategyNakedSingle::getName() const
{
   return "Naked single";
}
