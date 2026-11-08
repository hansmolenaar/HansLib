#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "Cell.h"
#include "SweepCombined.h"
#include "TestModels.h"
#include <iostream>

using namespace Sudoku;

namespace
{

CandidatesGrid GenerateSolvedGrid()
{
   const std::array<Value, NumField> diagramValues = {
       1, 2, 3, 4, 5, 6, 7, 8, 9, 4, 5, 6, 7, 8, 9, 1, 2, 3, 7, 8, 9, 1, 2, 3, 4, 5, 6,

       2, 3, 4, 5, 6, 7, 8, 9, 1, 5, 6, 7, 8, 9, 1, 2, 3, 4, 8, 9, 1, 2, 3, 4, 5, 6, 7,

       3, 4, 5, 6, 7, 8, 9, 1, 2, 6, 7, 8, 9, 1, 2, 3, 4, 5, 9, 1, 2, 3, 4, 5, 6, 7, 8,
   };

   CandidatesGrid result;

   for (auto field : FieldAll)
   {
      result.setGiven(field, diagramValues[field]);
   }
   return result;
}
} // namespace

TEST(CandidatesGridTest, DefaultConstructor)
{
   const CandidatesGrid cg;
   ASSERT_EQ(cg.getNumCandidates(), NumField * NumValue);
}

TEST(CandidatesGridTest, Solved)
{
   const auto solved = GenerateSolvedGrid();
   ASSERT_TRUE(solved.isSolved());
}

TEST(CandidatesGridTest, CheckValidOK)
{
   CandidatesGrid grid;
   ASSERT_NO_THROW(grid.checkValidSudoku());

   grid = GenerateSolvedGrid();
   ASSERT_NO_THROW(grid.checkValidSudoku());
}

TEST(CandidatesGridTest, CheckValidDoubleValue)
{
   CandidatesGrid grid;

   grid.setGiven(1, 1);
   ASSERT_NO_THROW(grid.checkValidSudoku());

   grid.setGiven(2, 2);
   ASSERT_NO_THROW(grid.checkValidSudoku());

   grid.setGiven(3, 1);
   ASSERT_ANY_THROW(grid.checkValidSudoku());
}

TEST(CandidatesGridTest, update)
{
   CandidatesGrid grid;

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
   auto found = grid[Cell::rowCol2Field(2, 1)].tryGetSolvedValue();
   ASSERT_TRUE(found.has_value());
   ASSERT_EQ(found.value(), 7);
}

TEST(CandidatesGridTest, SetCandidates)
{
   constexpr Field field = 2;
   CandidatesGrid cg;
   cg.setCandidates(field, {3, 4});
   ASSERT_EQ(cg.getNumCandidates(), (NumField - 1) * NumValue + 2);
   cg.setCandidates(field, {4});
   ASSERT_EQ(cg.getNumCandidates(), (NumField - 1) * NumValue + 1);
   cg.setCandidates(field, {4});
   ASSERT_EQ(cg.getNumCandidates(), (NumField - 1) * NumValue + 1);

   ASSERT_ANY_THROW(cg.setCandidates(field, {3}));
   ASSERT_ANY_THROW(cg.setCandidates(field, {}));
}

TEST(CandidatesGridTest, RemoveCandidate)
{
   constexpr Field field = 2;
   CandidatesGrid cg;

   cg.setCandidates(field, {3, 4});
   ASSERT_EQ(cg[field].size(), 2);
   ASSERT_TRUE(cg[field].contains(3));
   cg.removeCandidate(field, 3);
   ASSERT_FALSE(cg[field].contains(3));
   ASSERT_TRUE(cg[field].contains(4));
   ASSERT_EQ(cg[field].size(), 1);

   ASSERT_EQ(cg.getNumCandidates(), (NumField - 1) * NumValue + 1);

   ASSERT_NO_THROW(cg.removeCandidate(field, 3));  // OK, not a candidate anymore
   ASSERT_ANY_THROW(cg.removeCandidate(field, 4)); // only candidate
}

TEST(CandidatesGridTest, MesaageText)
{
   constexpr Field field = 2;
   constexpr Value value = 3;
   CandidatesGrid cg;
   cg.setGiven(field, value);
   const bool anyChange = cg.removeCandidate(field, value + 1);
   ASSERT_FALSE(anyChange);
   ASSERT_ANY_THROW(cg.removeCandidate(field, value));
}

TEST(CandidatesGridTest, ToOStream)
{
   CandidatesGrid cg;
   std::ostringstream stream;

   cg.setCandidates(9, {3, 4});
   cg.setCandidates(8, {8});
   cg.setCandidates(NumField - 1, {9});

   stream << cg;
   const auto retval = stream.str();
   // std::cout << retval;
   ASSERT_FALSE(retval.empty());
}

TEST(CandidatesGridTest, Symmetries)
{
   const std::string str = "020010840709200001604000290000030020800102004090040000037000602100005903062070050";
   CandidatesGrid grid = CandidatesGrid::FromString(str, '0');
   grid.update();

   const std::vector<CandidatesGrid> grids = CandidatesGrid::getSymmetricEquivalents(grid);
   // std::cout << grids[0] << "\n";
   // std::cout << grids[7] << "\n";

   const auto expect = grid.getNumCandidates();
   for (const auto &tg : grids)
   {
      ASSERT_EQ(tg.getNumCandidates(), expect);
      tg.checkValidSudoku();
   }
}

TEST(CandidatesGridTest, Simple)
{
   SweepCombined *sweepers = SweepCombined::getSimpleSweepers();
   std::vector<CandidatesGrid> grids = TestModels::getSimplePuzzles();
   ASSERT_EQ(grids.size(), 133);

   for (auto &grid : grids)
   {
      ISweep::iterate(*sweepers, grid);
      ASSERT_TRUE(grid.isSolved());
   }
}

TEST(CandidatesGridTest, Medium)
{
   // ASSERT_TRUE(false);
   SweepCombined *sweepers = SweepCombined::getMediumSweepers();
   std::vector<CandidatesGrid> grids = TestModels::getMediumPuzzles();
   ASSERT_EQ(grids.size(), 863);
   int numSolved = 0;
   for (auto &grid : grids)
   {
      ISweep::iterate(*sweepers, grid);
      if (grid.isSolved())
      {
         numSolved += 1;
      }
   }
   ASSERT_EQ(numSolved, 863);
}

TEST(CandidatesGridTest, Hard)
{
   // ASSERT_TRUE(false);
   SweepCombined *sweepers = SweepCombined::getHardSweepers();
   std::vector<CandidatesGrid> grids = TestModels::getHardPuzzles();
   ASSERT_EQ(grids.size(), 1278);
   int numSolved = 0;
   for (auto &grid : grids)
   {
      ISweep::iterate(*sweepers, grid);
      if (grid.isSolved())
      {
         numSolved += 1;
      }
   }
   ASSERT_EQ(numSolved, 610);
}
