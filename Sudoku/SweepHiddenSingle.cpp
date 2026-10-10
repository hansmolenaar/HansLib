#include "SweepHiddenSingle.h"
#include "Grid.h"
#include "House.h"

using namespace Sudoku;

namespace
{

std::optional<Field> tryFindUniqueCellForValue(const Grid &grid, const House &house, Value value)
{
   std::optional<Field> result;
   for (const auto &cell : house.cells)
   {
      if (grid.isCandidateInCell(cell.field, value))
      {
         if (result.has_value())
         {
            // More then one...
            return {};
         }
         const auto solved = grid.tryGetSolvedValue(cell.field);
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

void SweepHiddenSingle::sweep(Grid &grid)
{
   for (const House &house : House::getAllHouses())
   {
      for (const Value value : ValueAll)
      {
         std::optional<Field> candidateInField = tryFindUniqueCellForValue(grid, house, value);
         if (candidateInField.has_value())
         {
            // only cell with this value as candidate
            grid.setValue(candidateInField.value(), value);
         }
      }
   }
}
