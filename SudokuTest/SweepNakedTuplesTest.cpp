#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "Cell.h"
#include "SweepNakedTuples.h"

using namespace Sudoku;

TEST(SweepNakedTuplesTest, SweepNakedPair)
{
   CandidatesGrid grid;
   const Field Field22 = Cell::rowCol2Field(2, 2);
   grid.setGiven(1, 0, 9);
   grid.setGiven(2, 0, 6);
   grid.setGiven(2, 1, 8);

   grid.setCandidates(Cell::rowCol2Field(0, 0), {1, 2, 4, 5});
   grid.setCandidates(Cell::rowCol2Field(0, 1), {1, 2, 4, 5, 7});
   grid.setCandidates(Cell::rowCol2Field(0, 2), {2, 4, 5, 7});
   grid.setCandidates(Cell::rowCol2Field(1, 1), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(1, 2), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(2, 2), {2, 3, 5});

   size_t expectNumCandidates = 671;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);
   auto value0 = grid[Field22].tryGetSolvedValue();
   ASSERT_FALSE(value0.has_value());

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2});
   expectNumCandidates -= 2 * 6;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates); // Remainer of row removed

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2});
   value0 = grid[Field22].tryGetSolvedValue();
   ASSERT_TRUE(value0.has_value());
   ASSERT_EQ(value0.value(), 5);
   expectNumCandidates -= 5;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);
}

TEST(SweepNakedTuplesTest, SweepNakedPairAnotherExample)
{
   CandidatesGrid grid;
   const Index row = 8;
   const Field field0 = Cell::rowCol2Field(row, 0);
   const Field field7 = Cell::rowCol2Field(row, 7);
   grid.setGiven(row, 2, 4);
   grid.setGiven(row, 4, 8);
   grid.setGiven(row, 5, 7);
   grid.setGiven(row, 8, 2);

   grid.setCandidates(Cell::rowCol2Field(row, 0), {1, 3, 6, 9});
   grid.setCandidates(Cell::rowCol2Field(row, 1), {1, 5});
   grid.setCandidates(Cell::rowCol2Field(row, 3), {3, 6, 9});
   grid.setCandidates(Cell::rowCol2Field(row, 6), {1, 5});
   grid.setCandidates(Cell::rowCol2Field(row, 7), {1, 6});

   size_t expectNumCandidates = 665;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);
   auto value0 = grid[field7].tryGetSolvedValue();
   ASSERT_FALSE(value0.has_value());

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2});
   expectNumCandidates -= 2;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);
   ASSERT_EQ(grid[field7].tryGetSolvedValue().value(), 6);
   ASSERT_EQ(grid[field0].size(), 3);
}
