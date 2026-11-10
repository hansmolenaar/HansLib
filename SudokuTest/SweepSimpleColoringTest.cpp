#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "ISweep.h"
#include "Network.h"
#include "Single.h"
#include "StreamUtils.h"
#include "SweepCombined.h"
#include "SweepSimpleColoring.h"

#include <iostream>

using namespace Sudoku;
using namespace Utilities;

namespace
{
CandidatesGrid getSimpleExample()
{
   CandidatesGrid result;
   for (Cell cell : Cell::getAll())
   {
      const bool bot_top = cell.row == 0 || cell.row == 8;
      const bool left_right = cell.col == 0 || cell.col == 8;
      if (bot_top != left_right)
      {
         result.removeCandidate(cell.field, 1);
      }
   }

   return result;
}

}; // namespace

TEST(SweepSimpleColoringTest, BasicExample)
{
   const Value value = 1;
   CandidatesGrid grid = getSimpleExample();
   const auto links = SweepSimpleColoring::getBilocationLinks(grid, value);
   ASSERT_EQ(links.size(), 4);
   const auto networks = Network::create(links);
   const auto network = Single(networks);
   const auto coloring = Network::setColors(network);
   ASSERT_FALSE(coloring.empty());

   Field expect = 701;
   ASSERT_EQ(grid.getNumCandidates(), expect);
   const FieldFlags ignore;
   const bool anyChange = SweepSimpleColoring::removeSees2ColorsCandidates(grid, value, coloring, ignore);
   ASSERT_FALSE(anyChange);
}

TEST(SweepSimpleColoringTest, Sees2Colors)
{
   SweepCombined *sweepCombined = SweepCombined::getMediumSweepers();

   const std::string puzzle = "1.85..4.6....7.9..53...4..7..1.6...8.9.4.8.7.8...5.6..7..1...69..6.8....9.4..62.5";
   auto grid = CandidatesGrid::FromString(puzzle);
   ISweep::iterate(*sweepCombined, grid);
   ASSERT_EQ(grid.getNumCandidates(), 131);
   std::cout << grid;
   auto prv = grid;
   Cell changedCell = Cell::getFromRowCol(4, 8);
   ASSERT_TRUE(grid[changedCell.field].contains(1));
   const bool anyChange = SweepSimpleColoring::sweep(grid, 1);
   ASSERT_FALSE(grid[changedCell.field].contains(1));
   std::cout << grid;
   std::cout << CandidatesGrid::DifferenceToStrings(prv, grid);
   ASSERT_TRUE(anyChange);
   ASSERT_EQ(grid.getNumCandidates(), 130);
}
