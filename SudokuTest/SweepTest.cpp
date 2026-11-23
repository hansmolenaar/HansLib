#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "StrategyHiddenTuple.h"
#include "StrategyNakedSingle.h"
#include "Sweep.h"

using namespace Sudoku;

namespace
{
// from https://sudopedia.sudocue.net/index.php/Hidden_Single
// Should be solvable by naked and hidden singles
const std::string model_1 = "1..2..3...2..1..4...3..5..67..6..5...5..8..7...8..4..18..7..4...3..6..2...9..2..7";
const std::string model_2 = "..1...2.....3.4...5...6...1.5.1.2.7...8...4...2.9.7.8.9...7...8...5.9.....3...7..";
const std::string model_3 = "1...2...3.4.....1...5.4.6.....2.1...7.8...1.5...4.8.....7.6.8...8.....2.4...3...7";
const std::string model_4 = "1...2...3.4.....5....6.7.....5...8..7...8...2..6...9.....8.2....1.....6.2...7...1";
const std::string model_5 = "1..2..3......4..5...6..7..82.....7...5..1..4...7.....98..1..2...6..2......4..6..7";
const std::string model_6 = "...........12.34...235.617..35...74.....3.....76...82..143.825...87.23...........";

StrategyHiddenSingle hiddenSingle;
StrategyNakedSingle nakedSingle;
Sweep sweep(Strategies{&nakedSingle, &hiddenSingle});

} // namespace

TEST(SweepTest, SingleSweep)
{
   StrategyNakedSingle strategy;
   Sweep sweep(&strategy);
   CandidatesGrid grid;
   grid.setGiven(80, 9);

   FieldsInfo finfo(grid);
   const auto changes = sweep.singleSweep(grid, finfo);
   ASSERT_EQ(changes.numValuesChanged, 20);
   ASSERT_EQ(changes.numFieldsSolved, 0);
}

TEST(SweepTest, SingleSweepAndUpdate)
{
   StrategyNakedSingle strategy;
   Sweep sweep(&strategy);
   CandidatesGrid grid;
   grid.setCandidates(0, {1});
   grid.setCandidates(8, {1, 2});
   grid.setCandidates(5, {1, 2, 3});

   FieldsInfo finfo(grid);
   const auto anyChange = sweep.singleSweepAndUpdate(grid, finfo);
   ASSERT_TRUE(anyChange);
   ASSERT_EQ(finfo.getSolvedFields().count(), 3);
   ASSERT_EQ(finfo.getActiveFields().count(), 78);
}

TEST(SweepTest, Iterate)
{
   StrategyNakedSingle strategy;
   Sweep sweep(&strategy);
   CandidatesGrid grid;
   grid.setCandidates(0, {1});
   grid.setCandidates(8, {1, 2});
   grid.setCandidates(5, {1, 2, 3});
   grid.setCandidates(7, {1, 2, 3, 4});
   grid.setCandidates(3, {1, 2, 3, 4, 5});

   FieldsInfo finfo(grid);
   const auto anyChange = sweep.iterate(grid, finfo);
   ASSERT_TRUE(anyChange);
   ASSERT_EQ(finfo.getSolvedFields().count(), 5);
   ASSERT_EQ(finfo.getActiveFields().count(), 76);
}

TEST(SweepTest, SweepExample1)
{
   CandidatesGrid grid = CandidatesGrid::FromString(model_1);
   FieldsInfo finfo(grid);
   grid.update(finfo);
   ASSERT_EQ(grid.getNumCandidates(), 213);
   sweep.iterate(grid, finfo);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}

TEST(SweepTest, SweepExample2)
{
   CandidatesGrid grid = CandidatesGrid ::FromString(model_2);
   FieldsInfo finfo(grid);
   grid.update(finfo);
   ASSERT_EQ(grid.getNumCandidates(), 170);
   sweep.iterate(grid, finfo);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}

TEST(SweepTest, SweepExample3)
{
   CandidatesGrid grid = CandidatesGrid ::FromString(model_3);
   FieldsInfo finfo(grid);
   grid.update(finfo);
   ASSERT_EQ(grid.getNumCandidates(), 238);
   sweep.iterate(grid, finfo);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}

TEST(SweepTest, SweepExample4)
{
   CandidatesGrid grid = CandidatesGrid ::FromString(model_4);
   FieldsInfo finfo(grid);
   grid.update(finfo);
   ASSERT_EQ(grid.getNumCandidates(), 269);
   sweep.iterate(grid, finfo);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}

TEST(SweepTest, SweepExample5)
{
   CandidatesGrid grid = CandidatesGrid ::FromString(model_5);
   FieldsInfo finfo(grid);
   grid.update(finfo);
   ASSERT_EQ(grid.getNumCandidates(), 247);
   sweep.iterate(grid, finfo);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}

TEST(SweepTest, SweepExample6)
{
   CandidatesGrid grid = CandidatesGrid ::FromString(model_6);
   FieldsInfo finfo(grid);
   grid.update(finfo);
   ASSERT_EQ(grid.getNumCandidates(), 224);
   sweep.iterate(grid, finfo);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}
