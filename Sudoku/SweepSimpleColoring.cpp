#include "SweepSimpleColoring.h"
#include "CandidatesGrid.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"

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
