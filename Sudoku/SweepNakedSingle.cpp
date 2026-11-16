#include "SweepNakedSingle.h"
#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"

using namespace Sudoku;
using namespace Utilities;

bool SweepNakedSingle::sweep(CandidatesGrid &grid)
{
   std::array<FieldBits, NumValue + 1> toRemove;

   FieldsInfo finfo(grid);
   const auto &solvedFields = finfo.getSolvedFields();
   for (Field field : FieldAll | std::views::filter(Bitset{solvedFields}))
   {
      const Value value = grid[field].tryGetSolvedValue().value();
      toRemove[value] |= finfo.getVisibleFields(field);
   }

   bool anyChange = false;
   for (Value value : ValueAll)
   {
      anyChange |= grid.removeCandidateFromFields(toRemove.at(value), value);
   }
   return anyChange;
}
