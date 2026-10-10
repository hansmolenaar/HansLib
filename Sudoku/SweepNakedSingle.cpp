#include "SweepNakedSingle.h"
#include "Grid.h"
#include "House.h"

using namespace Sudoku;

namespace
{
template <typename F> void UnsetConnectedCells(Grid &grid, Field fieldSkip, Value value, F &&getHouse)
{
   const House &house = getHouse(fieldSkip);
   for (const auto &cell : house.cells)
   {
      if (cell.field != fieldSkip)
      {
         grid.unsetCandidate(cell.field, value);
      }
   }
}

} // namespace

void SweepNakedSingle::sweep(Grid &grid)
{
   for (Field field : FieldAll)
   {
      const auto value = grid.tryGetSolvedValue(field);
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
