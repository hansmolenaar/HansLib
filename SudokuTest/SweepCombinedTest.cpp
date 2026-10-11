#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "SweepCombined.h"
#include "SweepHiddenSingle.h"
#include "SweepNakedSingle.h"

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

SweepHiddenSingle hiddenSingle;
SweepNakedSingle nakedSingle;
SweepCombined sweepCombined({&nakedSingle, &hiddenSingle});

} // namespace

TEST(SweepCombinedTest, SweepExample1)
{
   CandidatesGrid grid = CandidatesGrid::FromString(model_1);
   grid.update();
   ASSERT_EQ(grid.getNumCandidates(), 213);
   ISweep::iterate(sweepCombined, grid);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}

TEST(SweepCombinedTest, SweepExample2)
{
   CandidatesGrid grid = CandidatesGrid ::FromString(model_2);
   grid.update();
   ASSERT_EQ(grid.getNumCandidates(), 170);
   ISweep::iterate(sweepCombined, grid);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}

TEST(SweepCombinedTest, SweepExample3)
{
   CandidatesGrid grid = CandidatesGrid ::FromString(model_3);
   grid.update();
   ASSERT_EQ(grid.getNumCandidates(), 238);
   ISweep::iterate(sweepCombined, grid);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}

TEST(SweepCombinedTest, SweepExample4)
{
   CandidatesGrid grid = CandidatesGrid ::FromString(model_4);
   grid.update();
   ASSERT_EQ(grid.getNumCandidates(), 269);
   ISweep::iterate(sweepCombined, grid);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}

TEST(SweepCombinedTest, SweepExample5)
{
   CandidatesGrid grid = CandidatesGrid ::FromString(model_5);
   grid.update();
   ASSERT_EQ(grid.getNumCandidates(), 247);
   ISweep::iterate(sweepCombined, grid);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}

TEST(SweepCombinedTest, SweepExample6)
{
   CandidatesGrid grid = CandidatesGrid ::FromString(model_6);
   grid.update();
   ASSERT_EQ(grid.getNumCandidates(), 224);
   ISweep::iterate(sweepCombined, grid);
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_TRUE(grid.isSolved());
}
