#include "SweepSimpleColoring.h"
#include "CandidatesGrid.h"
#include "ColorField.h"
#include "House.h"
#include "MyAssert.h"
#include "Network.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"
#include "Visibility.h"
#include <algorithm>
#include <iterator>
#include <ranges>

using namespace Sudoku;
using namespace Utilities;

std::vector<Link> SweepSimpleColoring::getBilocationLinks(const CandidatesGrid &grid, Value value)
{
   std::vector<Link> result;
   for (const House &house : House::getAllHouses())
   {
      const auto value2fields = SudokuUtils::getValue2Fields(grid, std::span(house.cells));
      const auto &fields = value2fields[value];
      if (fields.size() == 2)
      {
         result.emplace_back(Cell::get(fields.front()), Cell::get(fields.back()), LinkType::Strong);
      }
   }

   UniqueSorted(result);
   return result;
}

bool SweepSimpleColoring::removeSees2ColorsCandidates(CandidatesGrid &grid, Value value, const ColorField &colors,
                                                      const FieldBits &cellsToIgnore)

{
   const auto cellsGreen = colors.getCellsWithColor(Color::Green);
   const auto cellsBlue = colors.getCellsWithColor(Color::Blue);

   const std::vector<Cell> seesGreen = Visiblity::getVisibleCells(cellsGreen);
   const std::vector<Cell> seesBlue = Visiblity::getVisibleCells(cellsBlue);
   std::vector<Cell> seesBoth;
   std::set_intersection(seesGreen.begin(), seesGreen.end(), seesBlue.begin(), seesBlue.end(),
                         std::back_inserter(seesBoth));
   auto useCell = [&cellsToIgnore](const Cell &c) { return !cellsToIgnore.test(c.field); };
   auto activeCells = seesBoth | std::views::filter(useCell);
   const bool anyChange = grid.RemoveCandidate(value, activeCells);
   return anyChange;
}

bool SweepSimpleColoring::removeTwiceInAUnit(CandidatesGrid &grid, Value value, const ColorField &colors)
{
   for (Color color : ColorAll)
   {
      const auto cellsWithColor = colors.getCellsWithColor(color);
      if (cellsWithColor.size() < 2)
      {
         continue;
      }
      for (Field n = 0; n < cellsWithColor.size() - 1; ++n)
      {
         const Cell cell1 = cellsWithColor.at(n);
         const Cell cell2 = cellsWithColor.at(n + 1);
         if (Visiblity::areVisible(cell1, cell2))
         {
            if (grid.RemoveCandidate(value, cellsWithColor))
            {
               return true;
            }
         }
      }
   }
   return false;
}

bool SweepSimpleColoring::removeOneColorEmptiesHouse(CandidatesGrid &grid, Value value, const ColorField &colors,
                                                     const FieldBits &cellsToIgnore)
{
   const FieldBits fieldsValue = CandidatesGrid::getActiveFieldsWithValue(grid, value);
   const auto cellsBlue = colors.getCellsWithColor(Color::Blue);
   const auto cellsGreen = colors.getCellsWithColor(Color::Green);
   const auto fieldsBlue = SudokuUtils::getFields(cellsBlue);
   const auto fieldsGreen = SudokuUtils::getFields(cellsGreen);

   for (const House &house : House::getAllHouses())
   {
      const auto &fieldsInHouse = House::getFieldsInHouse(house.houseId);
      if ((fieldsInHouse & cellsToIgnore).any())
      {
         // House contains part of the chain, we can ignore it
         continue;
      }

      const auto cellsInHouseWithValue = fieldsValue & fieldsInHouse;
      const auto numCellsInHouseWithValue = cellsInHouseWithValue.count();
      if (numCellsInHouseWithValue == 0)
      {
         continue;
      }
      const auto visibleFields = Visiblity::getVisibleFields(cellsInHouseWithValue);
      const auto numBlueVisibleFromHouse = (visibleFields & fieldsBlue).count();
      const auto numGreenVisibleFromHouse = (visibleFields & fieldsGreen).count();
      if (numBlueVisibleFromHouse == numCellsInHouseWithValue)
      {
         if (grid.RemoveCandidate(value, cellsBlue))
         {
            return true;
         }
      }
      else if (numGreenVisibleFromHouse == numCellsInHouseWithValue)
      {
         if (grid.RemoveCandidate(value, cellsGreen))
         {
            return true;
         }
      }
   }
   return false;
}

bool SweepSimpleColoring::sweep(CandidatesGrid &grid, Value value)
{
   const auto links = getBilocationLinks(grid, value);
   if (links.empty())
   {
      return false;
   }
   const auto networks = Network::create(links);
   for (const auto &network : networks)
   {
      const auto coloring = Network::setColors(network);
      auto cellsToIgnore = SudokuUtils::getSolvedCells(grid);
      for (const auto &c : network.getCells())
      {
         cellsToIgnore.set(c.field);
      }

      if (SweepSimpleColoring::removeSees2ColorsCandidates(grid, value, coloring, cellsToIgnore))
      {
         // Better start from scratch
         return true;
      }
      if (SweepSimpleColoring::removeTwiceInAUnit(grid, value, coloring))
      {
         return true;
      }
      if (SweepSimpleColoring::removeOneColorEmptiesHouse(grid, value, coloring, cellsToIgnore))
      {
         MyAssert(false, "RemoveOneColorEmptiesHouse first time, not tested!!"); // TODO
         return true;
      }
   }

   return false;
}

bool SweepSimpleColoring::sweep(CandidatesGrid &grid)
{
   bool anyChange = false;
   for (Value value : ValueAll)
   {
      anyChange |= sweep(grid, value);
   }
   return anyChange;
}
