#include <gtest/gtest.h>

#include "Grid.h"
#include "TestModels.h"

#include <algorithm>
#include <sstream>

using namespace Sudoku;
namespace str = std::ranges;

TEST(GridTest, DefaultConstructor)
{
   const Grid grid;
   ASSERT_EQ(grid.getNumCandidates(), NumField * NumIndex);
   ASSERT_FALSE(grid.isSolved());
}

TEST(GridTest, SetGiven)
{
   Grid grid;
   grid.setValue(5, 7);
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

   grid.setValue(1, 9);
   stream.str("");
   stream << grid;
   retval = stream.str();
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == ValueUndefinedChar; }), NumField - 1);
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == '9'; }), 1);

   grid.setValue(10, 8);
   grid.setValue(20, 8);
   stream.str("");
   stream << grid;
   retval = stream.str();
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == ValueUndefinedChar; }), NumField - 3);
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == '9'; }), 1);
   ASSERT_EQ(str::count_if(retval, [](char c) { return c == '8'; }), 2);
}

TEST(GridTest, Solved)
{
   const Grid solved = TestModels::getSolved();
   ASSERT_TRUE(solved.isSolved());
}

TEST(GridTest, CheckValidOK)
{
   Grid grid;
   ASSERT_NO_THROW(grid.checkValidSudoku());

   grid = TestModels::getSolved();
   ASSERT_NO_THROW(grid.checkValidSudoku());
}

TEST(GridTest, CheckValidDoubleValue)
{
   Grid grid;

   grid.setValue(1, 1);
   ASSERT_NO_THROW(grid.checkValidSudoku());

   grid.setValue(2, 2);
   ASSERT_NO_THROW(grid.checkValidSudoku());

   grid.setValue(3, 1);
   ASSERT_ANY_THROW(grid.checkValidSudoku());
}

TEST(GridTest, update)
{
   Grid grid;

   grid.setValue(0, 0, 2);
   grid.setValue(0, 1, 5);
   grid.setValue(0, 2, 8);
   grid.setValue(1, 3, 5);
   grid.setValue(1, 4, 8);
   grid.setValue(1, 7, 7);
   grid.setValue(2, 0, 3);
   grid.setValue(2, 2, 9);
   grid.setValue(2, 4, 4);
   grid.setValue(4, 1, 6);
   grid.setValue(6, 1, 4);
   grid.setValue(6, 2, 6);

   grid.update();
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_EQ(grid.getNumCandidates(), 433);
   auto found = grid.tryGetSolvedValue(2, 1);
   ASSERT_TRUE(found.has_value());
   ASSERT_EQ(found.value(), 7);
}
