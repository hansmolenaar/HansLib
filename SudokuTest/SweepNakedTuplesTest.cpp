#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "Cell.h"
#include "SudokuDefines.h"
#include "SweepHiddenSingle.h"
#include "SweepNakedSingle.h"
#include "SweepNakedTuples.h"

using namespace Sudoku;

TEST(SweepNakedTuplesTest, SweepNakedPair_1)
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

   // std::cout << grid << "\n";
   size_t expectNumCandidates = 671;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);
   auto value0 = grid[Field22].tryGetSolvedValue();
   ASSERT_FALSE(value0.has_value());
   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2});
   // std::cout << grid << "\n";
   expectNumCandidates -= 2 * 6;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates); // Remainer of row removed

   // std::cout << grid << "\n";
   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2});
   // std::cout << grid << "\n";
   value0 = grid[Field22].tryGetSolvedValue();
   ASSERT_TRUE(value0.has_value());
   ASSERT_EQ(value0.value(), 5);
   expectNumCandidates -= 5;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);
}

TEST(SweepNakedTuplesTest, SweepNakedPair_2)
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

TEST(SweepNakedTuplesTest, SweepNakedTuples_1)
{
   const std::array<Value, NumField> diagramValues = {
       4, 0, 0, 0, 0, 0, 9, 3, 8, 0, 3, 2, 0, 9, 4, 1, 0, 0, 0, 9, 5, 3, 0, 0, 2, 4, 0,
       3, 7, 0, 6, 0, 9, 0, 0, 4, 5, 2, 9, 0, 0, 1, 6, 7, 3, 6, 0, 4, 7, 0, 3, 0, 9, 0,
       9, 5, 7, 0, 0, 8, 3, 0, 0, 0, 0, 3, 9, 0, 0, 4, 0, 0, 2, 4, 0, 0, 3, 0, 7, 0, 9,
   };

   auto grid = CandidatesGrid::FromValues(diagramValues);
   SweepNakedSingle().sweep(grid);
   Field expect = 152;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2, 3});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2, 3});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2, 3});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2});
   expect -= 6;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2});
   expect -= 7;
   ASSERT_EQ(grid.getNumCandidates(), expect);
}

TEST(SweepNakedTuplesTest, SweepNakedTuples_2)
{

   const std::array<Value, NumField> diagramValues = {
       9, 0, 8, 0, 0, 5, 4, 2, 3, 0, 0, 0, 0, 0, 4, 9, 5, 0, 0, 4, 0, 0, 0, 9, 0, 6, 0,

       0, 5, 0, 9, 3, 2, 8, 1, 6, 8, 1, 0, 5, 4, 0, 3, 0, 2, 0, 0, 2, 0, 8, 0, 5, 0, 4,

       0, 0, 3, 0, 0, 8, 0, 4, 5, 0, 0, 0, 0, 5, 3, 2, 8, 9, 0, 8, 0, 4, 0, 0, 0, 3, 0,
   };
   auto grid = CandidatesGrid::FromValues(diagramValues);

   SweepNakedSingle().sweep(grid);
   Field expect = 177;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepHiddenSingle().sweep(grid);
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2});
   expect -= 6;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2});
   expect -= 2;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2, 3});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2, 3});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2, 3});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expect);

   CandidatesGrid prv = grid;
   std::cout << prv << "\n";
   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2});
   std::cout << grid << "\n\n\n";
   std::cout << CandidatesGrid::Difference(prv, grid);
   ASSERT_EQ(grid.getNumCandidates(), expect);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expect);
}

TEST(SweepNakedTuplesTest, SweepNakedTuples_222_box)
{
   CandidatesGrid grid;

   grid.setCandidates(Cell::rowCol2Field(6, 6), {1, 2});
   grid.setCandidates(Cell::rowCol2Field(7, 7), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(8, 8), {3, 1});

   size_t expectNumCandidates = NumField * NumValue - 3 * 7;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2, 2});
   expectNumCandidates -= 6 * 3;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);
}

TEST(SweepNakedTuplesTest, SweepNakedTuples_222_row)
{
   CandidatesGrid grid;

   const Index row = 7;
   grid.setCandidates(Cell::rowCol2Field(row, 2), {1, 2});
   grid.setCandidates(Cell::rowCol2Field(row, 8), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(row, 5), {3, 1});

   size_t expectNumCandidates = NumField * NumValue - 3 * 7;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 2, 2});
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2, 2});
   expectNumCandidates -= 6 * 3;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);
}

TEST(SweepNakedTuplesTest, SweepNakedTuples_223_a)
{
   CandidatesGrid grid;

   grid.setCandidates(Cell::rowCol2Field(6, 6), {4, 5});
   grid.setCandidates(Cell::rowCol2Field(7, 7), {5, 6});
   grid.setCandidates(Cell::rowCol2Field(8, 8), {6, 4, 5});

   size_t expectNumCandidates = NumField * NumValue - 2 * 7 - 6;

   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Col, {2, 2, 3});
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Row, {2, 2, 3});
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples().sweepHouseOfType(grid, HouseType::Box, {2, 3, 2});
   expectNumCandidates -= 6 * 3;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);
}
