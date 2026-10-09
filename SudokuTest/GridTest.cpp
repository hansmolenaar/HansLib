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
   const Grid solved = TestModels::getSolved();
   ASSERT_TRUE(solved.isSolved());
}
