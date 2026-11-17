#include "SweepNakedSingle.h"
#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"

using namespace Sudoku;
using namespace Utilities;

std::set<FieldValue> SweepNakedSingle::collect(const FieldsInfo &finfo)
{
   std::set<FieldValue> result;
   const CandidatesGrid &grid = finfo.getGrid();
   const auto &solvedFields = finfo.getSolvedFields();
   for (Field field : FieldAll | std::views::filter(Bitset{solvedFields}))
   {
      const Value value = grid[field].tryGetSolvedValue().value();
      const auto visibleFields = finfo.getVisibleFields(field);
      for (Field field : FieldAll | std::views::filter(Bitset{visibleFields}))
      {
         result.insert({field, value});
      }
   }
   return result;
}
