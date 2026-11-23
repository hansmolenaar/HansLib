#include "StrategyNakedSingle.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "SudokuUtils.h"

using namespace Sudoku;
using namespace Utilities;

std::set<FieldValue> StrategyNakedSingle::collect(const FieldsInfo &finfo)
{
   FieldValue::Container result;
   const auto &solvedFields = finfo.getSolvedFields();

   const CandidatesGrid &grid = finfo.getGrid();
   for (Field field : SudokuUtils::FieldBitsToRange(solvedFields))
   {
      const Value value = grid[field].tryGetSolvedValue().value();
      auto visibleFields = finfo.getVisibleFields(field) & finfo.getActiveFieldsWithValue(value);
      FieldValue::add(visibleFields, value, result);
   }
   return result;
}
