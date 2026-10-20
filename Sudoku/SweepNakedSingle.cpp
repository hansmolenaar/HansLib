#include "SweepNakedSingle.h"
#include "CandidatesGrid.h"
#include "House.h"

#include <iostream>

using namespace Sudoku;

namespace
{
template <typename F> bool UnsetConnectedCells(CandidatesGrid &grid, Field fieldSkip, Value value, F &&getHouse)
{

   bool anyChange = false;
   const House &house = getHouse(fieldSkip);
   for (const auto &cell : house.cells)
   {
      if (cell.field != fieldSkip)
      {
         // TODO remove me
         // std::cout << "value to remove " << value << " fieldSkip = " << fieldSkip
         //          << "       cell.field = " << cell.field << " row = " << cell.row << " col = " << cell.col << "\n";
         // std::cout << grid << "\n";
         anyChange |= grid.removeCandidate(cell.field, value);
      }
   }
   return anyChange;
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
         anyChange |= UnsetConnectedCells(grid, field, value.value(),
                                          [](Field field) { return House::getRow(Cell::get(field).row); });
         anyChange |= UnsetConnectedCells(grid, field, value.value(),
                                          [](Field field) { return House::getCol(Cell::get(field).col); });
         anyChange |= UnsetConnectedCells(grid, field, value.value(),
                                          [](Field field) { return House::getBox(Cell::get(field).box); });
      }
   }
   return anyChange;
}
