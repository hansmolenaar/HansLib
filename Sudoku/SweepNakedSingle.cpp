#include "SweepNakedSingle.h"
#include "CandidatesGrid.h"
#include "House.h"

using namespace Sudoku;

namespace
{
template <typename F> void UnsetConnectedCells(CandidatesGrid &grid, Field fieldSkip, Value value, F &&getHouse)
{
   const House &house = getHouse(fieldSkip);
   for (const auto &cell : house.cells)
   {
      if (cell.field != fieldSkip)
      {
         grid.removeCandidate(cell.field, value);
      }
   }
}

} // namespace

void SweepNakedSingle::sweep(CandidatesGrid &grid)
{
   for (Field field : FieldAll)
   {
      const auto value = grid[field].tryGetSolvedValue();
      if (value)
      {
         UnsetConnectedCells(grid, field, value.value(),
                             [](Field field) { return House::getRow(Cell::get(field).row); });
         UnsetConnectedCells(grid, field, value.value(),
                             [](Field field) { return House::getCol(Cell::get(field).col); });
         UnsetConnectedCells(grid, field, value.value(),
                             [](Field field) { return House::getBox(Cell::get(field).box); });
      }
   }
}
