#include "SweepHiddenSingle.h"
#include "CandidatesGrid.h"
#include "House.h"

using namespace Sudoku;

namespace
{

std::optional<Field> tryFindUniqueCellForValue(const CandidatesGrid &grid, const House &house, Value value)
{
   std::optional<Field> result;
   for (const auto &cell : house.cells)
   {
      if (grid[cell.field].contains(value))
      {
         if (result.has_value())
         {
            // More then one...
            return {};
         }
         const auto solved = grid[cell.field].tryGetSolvedValue();
         if (solved.has_value())
         {
            // Already treated, ready
            return {};
         }
         // Set first occurence
         result = cell.field;
      }
   }
   return result;
}

} // namespace

void SweepHiddenSingle::sweep(CandidatesGrid &grid)
{
   for (const House &house : House::getAllHouses())
   {
      for (const Value value : ValueAll)
      {
         std::optional<Field> candidateInField = tryFindUniqueCellForValue(grid, house, value);
         if (candidateInField.has_value())
         {
            // only cell with this value as candidate
            grid.setCandidates(candidateInField.value(), {value});
         }
      }
   }
}
