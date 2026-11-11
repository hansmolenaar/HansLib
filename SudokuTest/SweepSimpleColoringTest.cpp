#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "ISweep.h"
#include "Network.h"
#include "Single.h"
#include "StreamUtils.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "SweepCombined.h"
#include "SweepSimpleColoring.h"
#include "TestModels.h"

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
   const FieldBits ignore;
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
   Cell changedCell = Cell::getFromRowCol(4, 8);
   ASSERT_TRUE(grid[changedCell.field].contains(1));
   const bool anyChange = SweepSimpleColoring::sweep(grid, 1);
   ASSERT_FALSE(grid[changedCell.field].contains(1));
   ASSERT_TRUE(anyChange);
   ASSERT_EQ(grid.getNumCandidates(), 130);
}

TEST(SweepSimpleColoringTest, TwiceInAUnit)
{
   SweepCombined *sweepCombined = SweepCombined::getMediumSweepers();
   SweepSimpleColoring sweepSimpleColoring;
   const std::string str = "2..9.....8..5.3....547.6..2..8.....9.97...42.1.....3..3..1.589....8.2..3.....7..5";
   CandidatesGrid grid = CandidatesGrid::FromString(str);
   ISweep::iterate(*sweepCombined, grid);

   const Value value = 4; // 6
   const auto links = SweepSimpleColoring::getBilocationLinks(grid, value);

   const auto networks = Network::create(links);
   bool anyChange = false;
   for (const auto &network : networks)
   {
      const auto coloring = Network::setColors(network);
      // std::cout << network << "\n\n";
      // std::cout << coloring << "\n";
      // std::cout << grid << "\n";
      // auto prv = grid;
      anyChange |= SweepSimpleColoring::removeTwiceInAUnit(grid, value, coloring);
      // std::cout << grid;
      // std::cout << CandidatesGrid::DifferenceToStrings(prv, grid);
   }
   ASSERT_TRUE(anyChange);
}

#if false
TEST(SweepSimpleColoringTest, ColorEmptiesHouse)
{
   SweepCombined *sweepCombined = SweepCombined::getMediumSweepers();
   SweepSimpleColoring sweepSimpleColoring;
   const std::string str = ".24...65.1.......7..8.1.9...........26..9..83.8.5.1.7.6..9.3..8..28547......7....";
   CandidatesGrid grid = CandidatesGrid::FromString(str);
   ISweep::iterate(*sweepCombined, grid);

   // const Value value = 1;
   for (Value value : ValueAll)
   {
      const auto links = SweepSimpleColoring::getBilocationLinks(grid, value);

      const auto networks = Network::create(links);
      bool anyChange = false;
      for (const auto &network : networks)
      {
         const auto coloring = Network::setColors(network);
         const auto coloredFields = SudokuUtils::getFields(network.getCells());
         // std::cout << str << "\n";
         std::cout << str << "\n";
         std::cout << value << "\n";
         std::cout << network << "\n\n";
         std::cout << coloring << "\n";
         std::cout << grid << "\n";
         auto prv = grid;
         anyChange |= SweepSimpleColoring::removeOneColorEmptiesHouse(grid, value, coloring, coloredFields);
         std::cout << grid;
         std::cout << CandidatesGrid::DifferenceToStrings(prv, grid);
         ASSERT_FALSE(anyChange);
      }
      ASSERT_TRUE(anyChange);
   }
}
#endif
