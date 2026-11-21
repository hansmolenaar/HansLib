#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "FieldValue.h"
#include "FieldsInfo.h"
#include "GridChange.h"
#include "ISweep.h"
#include "Network.h"
#include "Single.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "SweepCombined.h"
#include "SweepSimpleColoring.h"
#include "TestUtils.h"

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
   FieldsInfo finfo(grid);
   const auto links = SweepSimpleColoring::getBilocationLinks(grid, finfo, value);

   ASSERT_EQ(links.size(), 4);
   const auto networks = Network::create(links);
   const auto network = Single(networks);
   const auto coloring = Network::setColors(network);
   ASSERT_FALSE(coloring.empty());

   Field expect = 701;
   ASSERT_EQ(grid.getNumCandidates(), expect);
   const FieldBits ignore;
   const auto fieldValues = SweepSimpleColoring::collectSees2ColorsCandidates(finfo, value, coloring, ignore);
   ASSERT_TRUE(fieldValues.empty());
}

TEST(SweepSimpleColoringTest, Sees2Colors)
{
   SweepCombined *sweepCombined = SweepCombined::getMediumSweepers();

   const std::string puzzle = "1.85..4.6....7.9..53...4..7..1.6...8.9.4.8.7.8...5.6..7..1...69..6.8....9.4..62.5";
   auto grid = CandidatesGrid::FromString(puzzle);
   FieldsInfo finfo(grid);
   ISweep::iterate(*sweepCombined, grid, finfo);
   ASSERT_EQ(grid.getNumCandidates(), 131);
   Cell changedCell = Cell::getFromRowCol(4, 8);
   ASSERT_TRUE(grid[changedCell.field].contains(1));
   const bool anyChange = SweepSimpleColoring().sweep(grid, finfo);
   ASSERT_FALSE(grid[changedCell.field].contains(1));
   ASSERT_TRUE(anyChange);
   ASSERT_EQ(grid.getNumCandidates(), 129);
}

TEST(SweepSimpleColoringTest, TwiceInAUnit)
{
   SweepCombined *sweepCombined = SweepCombined::getMediumSweepers();
   SweepSimpleColoring sweepSimpleColoring;
   const std::string str = "2..9.....8..5.3....547.6..2..8.....9.97...42.1.....3..3..1.589....8.2..3.....7..5";
   CandidatesGrid grid = CandidatesGrid::FromString(str);
   FieldsInfo finfo(grid);
   ISweep::iterate(*sweepCombined, grid, finfo);

   const Value value = 4; // 6
   const auto links = SweepSimpleColoring::getBilocationLinks(grid, finfo, value);

   const auto networks = Network::create(links);
   FieldValue::Container fieldValues;
   for (const auto &network : networks)
   {
      const auto coloring = Network::setColors(network);

      FieldValue::add(SweepSimpleColoring::collectTwiceInAUnit(finfo, value, coloring), fieldValues);
   }
   ASSERT_EQ(fieldValues.size(), 4);
   TestUtils::CheckFieldValues(grid, fieldValues);
   finfo.update();
   const auto changes = grid.apply(fieldValues, finfo);
   ASSERT_EQ(changes.numValuesChanged, 4);
   ASSERT_EQ(changes.numFieldsSolved, 3);
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

TEST(SweepSimpleColoringTest, SingleColorEmptiesHouseExample1)
{
   SweepSimpleColoring sweepSimpleColoring;

   CandidatesGrid grid;
   grid.setGiven(Cell::rowCol2Field(0, 3), 7);
   grid.setGiven(Cell::rowCol2Field(0, 6), 5);

   grid.setGiven(Cell::rowCol2Field(1, 0), 3);
   grid.setGiven(Cell::rowCol2Field(1, 5), 1);
   grid.setGiven(Cell::rowCol2Field(1, 6), 4);
   grid.setGiven(Cell::rowCol2Field(1, 8), 7);

   grid.setGiven(Cell::rowCol2Field(2, 0), 7);
   grid.setGiven(Cell::rowCol2Field(2, 1), 1);
   grid.setGiven(Cell::rowCol2Field(2, 7), 6);

   grid.setGiven(Cell::rowCol2Field(3, 3), 1);
   grid.setGiven(Cell::rowCol2Field(3, 4), 8);
   grid.setGiven(Cell::rowCol2Field(3, 5), 5);

   grid.setGiven(Cell::rowCol2Field(4, 0), 5);
   grid.setGiven(Cell::rowCol2Field(4, 3), 9);
   grid.setGiven(Cell::rowCol2Field(4, 4), 2);
   grid.setGiven(Cell::rowCol2Field(4, 5), 4);
   grid.setGiven(Cell::rowCol2Field(4, 8), 3);

   grid.setGiven(Cell::rowCol2Field(5, 3), 3);
   grid.setGiven(Cell::rowCol2Field(5, 4), 6);
   grid.setGiven(Cell::rowCol2Field(5, 5), 7);
   grid.setGiven(Cell::rowCol2Field(5, 8), 5);

   grid.setGiven(Cell::rowCol2Field(6, 1), 6);
   grid.setGiven(Cell::rowCol2Field(6, 4), 7);
   grid.setGiven(Cell::rowCol2Field(6, 6), 2);
   grid.setGiven(Cell::rowCol2Field(6, 7), 5);

   grid.setGiven(Cell::rowCol2Field(7, 2), 7);
   grid.setGiven(Cell::rowCol2Field(7, 3), 5);
   grid.setGiven(Cell::rowCol2Field(7, 8), 8);

   grid.setGiven(Cell::rowCol2Field(8, 1), 5);
   grid.setGiven(Cell::rowCol2Field(8, 2), 4);
   grid.setGiven(Cell::rowCol2Field(8, 5), 2);

   FieldsInfo finfo(grid);
   grid.update(finfo);
   auto expect = 207;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   const Value value = 6;
   const auto links = SweepSimpleColoring::getBilocationLinks(grid, finfo, value);

   const auto networks = Network::create(links);
   ASSERT_EQ(networks.size(), 4);
   FieldValue::Container fieldValues;
   for (const auto &network : networks)
   {
      const auto coloring = Network::setColors(network);
      const auto coloredFields = network.getFields();

      const auto emptiesHosue = SweepSimpleColoring::collectOneColorEmptiesHouse(finfo, value, coloring, coloredFields);
      fieldValues.insert(emptiesHosue.begin(), emptiesHosue.end());
   }

   TestUtils::CheckFieldValues(grid, fieldValues);
   ASSERT_EQ(fieldValues.size(), 4);
   expect -= 4;
   finfo.update();
   const auto changes = grid.apply(fieldValues, finfo);
   ASSERT_EQ(changes.numValuesChanged, 4);
   ASSERT_EQ(changes.numFieldsSolved, 1);
   ASSERT_EQ(grid.getNumCandidates(), expect);
}

TEST(SweepSimpleColoringTest, SingleColorEmptiesHouseExample2)
{
   SweepSimpleColoring sweepSimpleColoring;

   CandidatesGrid grid;

   grid.setGiven(Cell::rowCol2Field(0, 3), 5);
   grid.setGiven(Cell::rowCol2Field(0, 4), 1);
   grid.setGiven(Cell::rowCol2Field(0, 5), 4);
   grid.setGiven(Cell::rowCol2Field(0, 6), 8);
   grid.setGiven(Cell::rowCol2Field(0, 7), 9);
   grid.setGiven(Cell::rowCol2Field(0, 8), 7);

   grid.setGiven(Cell::rowCol2Field(1, 0), 5);
   grid.setGiven(Cell::rowCol2Field(1, 1), 1);
   grid.setGiven(Cell::rowCol2Field(1, 2), 9);
   grid.setGiven(Cell::rowCol2Field(1, 6), 6);
   grid.setGiven(Cell::rowCol2Field(1, 7), 4);
   grid.setGiven(Cell::rowCol2Field(1, 8), 2);

   grid.setGiven(Cell::rowCol2Field(2, 0), 8);
   grid.setGiven(Cell::rowCol2Field(2, 1), 4);
   grid.setGiven(Cell::rowCol2Field(2, 2), 7);
   grid.setGiven(Cell::rowCol2Field(2, 3), 9);
   grid.setGiven(Cell::rowCol2Field(2, 4), 6);
   grid.setGiven(Cell::rowCol2Field(2, 5), 2);
   grid.setGiven(Cell::rowCol2Field(2, 6), 1);
   grid.setGiven(Cell::rowCol2Field(2, 7), 3);
   grid.setGiven(Cell::rowCol2Field(2, 8), 5);

   grid.setGiven(Cell::rowCol2Field(3, 3), 6);
   grid.setGiven(Cell::rowCol2Field(3, 4), 8);
   grid.setGiven(Cell::rowCol2Field(3, 5), 7);
   grid.setGiven(Cell::rowCol2Field(3, 7), 5);
   grid.setGiven(Cell::rowCol2Field(3, 8), 1);

   grid.setGiven(Cell::rowCol2Field(4, 0), 1);
   grid.setGiven(Cell::rowCol2Field(4, 1), 7);
   grid.setGiven(Cell::rowCol2Field(4, 2), 8);
   grid.setGiven(Cell::rowCol2Field(4, 4), 5);
   grid.setGiven(Cell::rowCol2Field(4, 6), 4);

   grid.setGiven(Cell::rowCol2Field(5, 1), 6);
   grid.setGiven(Cell::rowCol2Field(5, 2), 5);
   grid.setGiven(Cell::rowCol2Field(5, 3), 4);
   grid.setGiven(Cell::rowCol2Field(5, 5), 1);
   grid.setGiven(Cell::rowCol2Field(5, 6), 7);
   grid.setGiven(Cell::rowCol2Field(5, 7), 8);

   grid.setGiven(Cell::rowCol2Field(6, 0), 7);
   grid.setGiven(Cell::rowCol2Field(6, 3), 1);
   grid.setGiven(Cell::rowCol2Field(6, 5), 5);
   grid.setGiven(Cell::rowCol2Field(6, 8), 8);

   grid.setGiven(Cell::rowCol2Field(7, 1), 8);
   grid.setGiven(Cell::rowCol2Field(7, 2), 3);
   grid.setGiven(Cell::rowCol2Field(7, 6), 5);
   grid.setGiven(Cell::rowCol2Field(7, 7), 1);
   grid.setGiven(Cell::rowCol2Field(7, 8), 4);

   grid.setGiven(Cell::rowCol2Field(8, 1), 5);
   grid.setGiven(Cell::rowCol2Field(8, 2), 1);
   grid.setGiven(Cell::rowCol2Field(8, 7), 7);

   grid.checkValidSudoku();
   FieldsInfo finfo(grid);
   grid.update(finfo);

   ASSERT_TRUE(grid[Cell::rowCol2Field(5, 0)].contains(2));
   ASSERT_TRUE(grid[Cell::rowCol2Field(4, 3)].contains(2));
   ASSERT_TRUE(grid[Cell::rowCol2Field(3, 6)].contains(2));
   ASSERT_TRUE(grid[Cell::rowCol2Field(6, 7)].contains(2));

   Field expect = 137;
   ASSERT_EQ(grid.getNumCandidates(), expect);
   const Value value = 2;
   const auto links = SweepSimpleColoring::getBilocationLinks(grid, finfo, value);

   const auto networks = Network::create(links);
   ASSERT_EQ(networks.size(), 1);
   const auto &network = networks.front();
   const auto coloring = Network::setColors(network);
   const auto coloredFields = network.getFields();
   const auto fieldValues = SweepSimpleColoring::collectOneColorEmptiesHouse(finfo, value, coloring, coloredFields);
   ASSERT_EQ(fieldValues.size(), 4);
   expect -= 4;

   finfo.update();
   grid.apply(fieldValues, finfo);
   ASSERT_EQ(grid.getNumCandidates(), expect);

   ASSERT_FALSE(grid[Cell::rowCol2Field(5, 0)].contains(2));
   ASSERT_FALSE(grid[Cell::rowCol2Field(4, 3)].contains(2));
   ASSERT_FALSE(grid[Cell::rowCol2Field(3, 6)].contains(2));
   ASSERT_FALSE(grid[Cell::rowCol2Field(6, 7)].contains(2));
}

#if false
TEST(SweepSimpleColoringTest, FindColorEmptiesHouse)
{
   std::ifstream file("/home/hans/src/Top_50K_Toughest.txt");
   SweepSimpleColoring sweepSimpleColoring;

   // String to store each line of the file.
   std::string line;

   if (file.is_open())
   {
      // Read each line from the file and store it in the
      // 'line' variable.
      while (getline(file, line))
      {
         CandidatesGrid grid = CandidatesGrid::FromString(line.substr(0, NumField), '0');
         grid.update();
         std::cout << line << "\n";
         sweepSimpleColoring.sweep(grid);
      }

      // Close the file stream once all lines have been
      // read.
      file.close();
   }
   else
   {
      // Print an error message to the standard error
      // stream if the file cannot be opened.
      std::cerr << "Unable to open file!" << std::endl;
   }
}
#endif
