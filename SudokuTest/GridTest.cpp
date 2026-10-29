#include <gtest/gtest.h>

#include "Grid.h"
#include "SweepCombined.h"
#include "TestModels.h"

#include <algorithm>
#include <sstream>
#include <vector>

using namespace Sudoku;
namespace str = std::ranges;

namespace
{

Grid GenerateSolvedGrid()
{
   const std::array<Value, NumField> diagramValues = {
       1, 2, 3, 4, 5, 6, 7, 8, 9, 4, 5, 6, 7, 8, 9, 1, 2, 3, 7, 8, 9, 1, 2, 3, 4, 5, 6,

       2, 3, 4, 5, 6, 7, 8, 9, 1, 5, 6, 7, 8, 9, 1, 2, 3, 4, 8, 9, 1, 2, 3, 4, 5, 6, 7,

       3, 4, 5, 6, 7, 8, 9, 1, 2, 6, 7, 8, 9, 1, 2, 3, 4, 5, 9, 1, 2, 3, 4, 5, 6, 7, 8,
   };

   Grid result;

   for (auto field : FieldAll)
   {
      result.setGiven(field, diagramValues[field]);
   }
   return result;
}
} // namespace

TEST(GridTest, DefaultConstructor)
{
   const Grid grid;
   ASSERT_EQ(grid.getNumCandidates(), NumField * NumIndex);
   ASSERT_FALSE(grid.isSolved());
}

TEST(GridTest, SetGiven)
{
   Grid grid;
   grid.setGiven(5, 7);
   ASSERT_EQ(grid.getNumCandidates(), (NumField - 1) * NumIndex + 1);
   ASSERT_EQ(grid.tryGetSolvedValue(5).value(), 7);
   ASSERT_FALSE(grid.tryGetSolvedValue(7).has_value());
}

TEST(GridTest, ToOStream)
{
   Grid grid;

   std::ostringstream stream;
   stream << grid;
   std::string retval = stream.str();
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == ValueUndefinedChar; }), NumField);

   grid.setGiven(1, 9);
   stream.str("");
   stream << grid;
   retval = stream.str();
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == ValueUndefinedChar; }), NumField - 1);
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == '9'; }), 1);

   grid.setGiven(10, 8);
   grid.setGiven(20, 8);
   stream.str("");
   stream << grid;
   retval = stream.str();
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == ValueUndefinedChar; }), NumField - 3);
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == '9'; }), 1);
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == '8'; }), 2);
}

TEST(GridTest, Solved)
{
   const Grid solved = GenerateSolvedGrid();
   ASSERT_TRUE(solved.isSolved());
}

TEST(GridTest, CheckValidOK)
{
   Grid grid;
   ASSERT_NO_THROW(grid.checkValidSudoku());

   grid = GenerateSolvedGrid();
   ASSERT_NO_THROW(grid.checkValidSudoku());
}

TEST(GridTest, CheckValidDoubleValue)
{
   Grid grid;

   grid.setGiven(1, 1);
   ASSERT_NO_THROW(grid.checkValidSudoku());

   grid.setGiven(2, 2);
   ASSERT_NO_THROW(grid.checkValidSudoku());

   grid.setGiven(3, 1);
   ASSERT_ANY_THROW(grid.checkValidSudoku());
}

TEST(GridTest, update)
{
   Grid grid;

   grid.setGiven(0, 0, 2);
   grid.setGiven(0, 1, 5);
   grid.setGiven(0, 2, 8);
   grid.setGiven(1, 3, 5);
   grid.setGiven(1, 4, 8);
   grid.setGiven(1, 7, 7);
   grid.setGiven(2, 0, 3);
   grid.setGiven(2, 2, 9);
   grid.setGiven(2, 4, 4);
   grid.setGiven(4, 1, 6);
   grid.setGiven(6, 1, 4);
   grid.setGiven(6, 2, 6);

   grid.update();
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_EQ(grid.getNumCandidates(), 433);
   auto found = grid.tryGetSolvedValue(2, 1);
   ASSERT_TRUE(found.has_value());
   ASSERT_EQ(found.value(), 7);
}

TEST(GridTest, Simple)
{
   SweepCombined *sweepers = SweepCombined::getSimpleSweepers();
   std::vector<Grid> grids = TestModels::getSimplePuzzles();
   ASSERT_EQ(grids.size(), 133);

   for (auto &grid : grids)
   {
      ASSERT_TRUE(grid.solve(*sweepers));
      ASSERT_TRUE(grid.isSolved());
   }
}

TEST(GridTest, Medium)
{
   SweepCombined *sweepers = SweepCombined::getMediumSweepers();
   std::vector<Grid> grids = TestModels::getMediumPuzzles();
   ASSERT_EQ(grids.size(), 863);
   int numSolved = 0;
   for (auto &grid : grids)
   {
      if (grid.solve(*sweepers))
      {
         numSolved += 1;
      }
   }
   ASSERT_EQ(numSolved, 863);
}

TEST(GridTest, Hard)
{
   SweepCombined *simpleSweepers = SweepCombined::getMediumSweepers();
   std::vector<Grid> grids = TestModels::getHardPuzzles();
   ASSERT_EQ(grids.size(), 1278);
   int numSolved = 0;
   for (auto &grid : grids)
   {
      if (grid.solve(*simpleSweepers))
      {
         numSolved += 1;
      }
   }
   ASSERT_EQ(numSolved, 567);
}
