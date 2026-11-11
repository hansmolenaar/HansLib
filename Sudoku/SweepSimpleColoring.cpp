#include "SweepSimpleColoring.h"
#include "CandidatesGrid.h"
#include "ColorField.h"
#include "House.h"
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
