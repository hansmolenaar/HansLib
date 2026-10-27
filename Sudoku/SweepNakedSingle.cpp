#include "SweepNakedSingle.h"
#include "CandidatesGrid.h"
#include "Functors.h"
#include "House.h"

using namespace Sudoku;

namespace
{
bool UnsetConnectedCells(CandidatesGrid &grid, const Cell &cellSkip, Value value, const House &house)
{
   return grid.RemoveCandidates(std::span<Value>(&value, &value + 1), house.cells, Functors::IsNotEqualTo{cellSkip});
}

} // namespace

bool SweepNakedSingle::sweep(CandidatesGrid &grid)
{
   bool anyChange = false;
   for (Field field : FieldAll)
   {
      const auto value = grid[field].tryGetSolvedValue();
      if (value)
      {
         const Cell cell = Cell::get(field);
         anyChange |= UnsetConnectedCells(grid, cell, value.value(), House::getRow(cell.row));
         anyChange |= UnsetConnectedCells(grid, cell, value.value(), House::getCol(cell.col));
         anyChange |= UnsetConnectedCells(grid, cell, value.value(), House::getBox(cell.box));
      }
   }
   return anyChange;
}
