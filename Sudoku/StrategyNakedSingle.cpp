#include "StrategyNakedSingle.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"
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
      result.add(visibleFields, value);
   }
}
