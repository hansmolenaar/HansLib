#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "Cell.h"
#include "FieldsInfo.h"
#include "House.h"
#include "ISweep.h"
#include "SudokuDefines.h"
#include "Sweep.h"
#include "SweepNakedTuples.h"
#include "TestUtils.h"

using namespace Sudoku;

TEST(SweepNakedTuplesTest, Strategy)
{
   SweepNakedTuples strategy;
   TestUtils::CheckStrategyInterface(strategy);
}

TEST(SweepNakedTuplesTest, Pair_1)
{
   SweepNakedTuples strategy;
   Sweep sweep({&strategy});
   CandidatesGrid grid;

   grid.setCandidates(Cell::rowCol2Field(0, 0), {1, 2});
   grid.setCandidates(Cell::rowCol2Field(0, 1), {1, 2});

   FieldsInfo finfo(grid);
   auto fvs = SweepNakedTuples::collectHouse(finfo, House{HouseType::Row, 0}, 2);
   ASSERT_EQ(fvs.size(), 14);

   fvs = SweepNakedTuples::collectHouse(finfo, House{HouseType::Box, 0}, 2);
   ASSERT_EQ(fvs.size(), 14);

   fvs = SweepNakedTuples::collectOfTupleSize(finfo, 2);
   ASSERT_EQ(fvs.size(), 26);

   TestUtils::CheckStrategy(strategy, grid, GridChange{26, 0});
}

TEST(SweepNakedTuplesTest, NakedPair_2)
{
   SweepNakedTuples strategy;
   Sweep sweep({&strategy});
   CandidatesGrid grid;
   grid.setCandidates(Cell::rowCol2Field(0, 0), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(1, 1), {3, 2});

   FieldsInfo finfo(grid);

   auto fvs = SweepNakedTuples::collectHouse(finfo, House{HouseType::Row, 1}, 2);
   ASSERT_EQ(fvs.size(), 0);

   fvs = SweepNakedTuples::collectHouse(finfo, House{HouseType::Box, 0}, 2);
   ASSERT_EQ(fvs.size(), 14);

   fvs = SweepNakedTuples::collectOfTupleSize(finfo, 2);
   ASSERT_EQ(fvs.size(), 14);

   TestUtils::CheckStrategy(strategy, grid, GridChange{14, 0});
}

TEST(SweepNakedTuplesTest, SweepNakedPair_2)
{
   SweepNakedTuples strategy;
   Sweep sweep({&strategy});
   CandidatesGrid grid;
   const Index row = 8;
   grid.setGiven(row, 2, 4);
   grid.setGiven(row, 4, 8);
   grid.setGiven(row, 5, 7);
   grid.setGiven(row, 8, 2);

   grid.setCandidates(Cell::rowCol2Field(row, 0), {1, 3, 6, 9});
   grid.setCandidates(Cell::rowCol2Field(row, 1), {1, 5});
   grid.setCandidates(Cell::rowCol2Field(row, 3), {3, 6, 9});
   grid.setCandidates(Cell::rowCol2Field(row, 6), {1, 5});
   grid.setCandidates(Cell::rowCol2Field(row, 7), {1, 6});

   FieldsInfo finfo(grid);
   // TestUtils::ShowStrategy(strategy, finfo);

   auto fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Col, 2);
   EXPECT_EQ(fieldValues.size(), 0);

   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Box, 2);
   EXPECT_EQ(fieldValues.size(), 0);

   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Row, 2);
   EXPECT_EQ(fieldValues.size(), 2);

   fieldValues = SweepNakedTuples::collectOfTupleSize(finfo, 2); // 1 5
   EXPECT_EQ(fieldValues.size(), 2);

   fieldValues = SweepNakedTuples::collectOfTupleSize(finfo, 3); // 1 5 6
   EXPECT_EQ(fieldValues.size(), 3);

   TestUtils::CheckStrategy(strategy, grid, {4, 1});
}

TEST(SweepNakedTuplesTest, SweepNakedTuples_1)
{
   const std::array<Value, NumField> diagramValues = {
       4, 0, 0, 0, 0, 0, 9, 3, 8, 0, 3, 2, 0, 9, 4, 1, 0, 0, 0, 9, 5, 3, 0, 0, 2, 4, 0,
       3, 7, 0, 6, 0, 9, 0, 0, 4, 5, 2, 9, 0, 0, 1, 6, 7, 3, 6, 0, 4, 7, 0, 3, 0, 9, 0,
       9, 5, 7, 0, 0, 8, 3, 0, 0, 0, 0, 3, 9, 0, 0, 4, 0, 0, 2, 4, 0, 0, 3, 0, 7, 0, 9,
   };

   auto grid = CandidatesGrid::FromValues(diagramValues);
   FieldsInfo finfo(grid);
   grid.update(finfo);
   Field expect = 152;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   auto fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Col, 3);
   ASSERT_TRUE(fieldValues.empty());

   auto changes = grid.apply(fieldValues, finfo);
   ASSERT_EQ(changes.numValuesChanged, 0);

   SweepNakedTuples strategy;
   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Row, 3);
   ASSERT_TRUE(fieldValues.empty());

   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Box, 3);
   ASSERT_EQ(grid.getNumCandidates(), expect);

   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Col, 2);
   ASSERT_EQ(grid.getNumCandidates(), expect);

   Sweep sweep({&strategy});
   TestUtils::CheckStrategy(strategy, grid, {13, 1});
}

TEST(SweepNakedTuplesTest, Box_222)
{
   CandidatesGrid grid;

   grid.setCandidates(Cell::rowCol2Field(6, 6), {1, 2});
   grid.setCandidates(Cell::rowCol2Field(7, 7), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(8, 8), {3, 1});

   size_t expectNumCandidates = NumField * NumValue - 3 * 7;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples strategy;
   FieldsInfo finfo(grid);
   auto fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Col, 3);
   ASSERT_TRUE(fieldValues.empty());

   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Row, 3);
   ASSERT_TRUE(fieldValues.empty());

   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Box, 3);
   ASSERT_EQ(fieldValues.size(), 3 * 6);

   TestUtils::CheckStrategy(strategy, grid, {18, 0});
}

TEST(SweepNakedTuplesTest, Row_222)
{
   CandidatesGrid grid;

   const Index row = 7;
   grid.setCandidates(Cell::rowCol2Field(row, 2), {1, 2});
   grid.setCandidates(Cell::rowCol2Field(row, 8), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(row, 5), {3, 1});

   size_t expectNumCandidates = NumField * NumValue - 3 * 7;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples strategy;
   FieldsInfo finfo(grid);
   auto fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Col, 3);
   ASSERT_TRUE(fieldValues.empty());

   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Box, 3);
   ASSERT_TRUE(fieldValues.empty());

   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Row, 3);
   ASSERT_EQ(fieldValues.size(), 3 * 6);

   TestUtils::CheckStrategy(strategy, grid, {18, 0});
}

TEST(SweepNakedTuplesTest, SweepNakedTuples_223_a)
{
   CandidatesGrid grid;

   grid.setCandidates(Cell::rowCol2Field(6, 6), {4, 5});
   grid.setCandidates(Cell::rowCol2Field(7, 7), {5, 6});
   grid.setCandidates(Cell::rowCol2Field(8, 8), {6, 4, 5});

   size_t expectNumCandidates = NumField * NumValue - 2 * 7 - 6;

   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples strategy;
   FieldsInfo finfo(grid);
   // TestUtils::ShowStrategy(strategy, finfo);

   auto fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Col, 3);
   ASSERT_TRUE(fieldValues.empty());

   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Row, 3);
   ASSERT_TRUE(fieldValues.empty());

   fieldValues = SweepNakedTuples::collectHousesOfType(finfo, HouseType::Box, 3);
   ASSERT_EQ(fieldValues.size(), 3 * 6);

   TestUtils::CheckStrategy(strategy, grid, {18, 0});
}

TEST(SweepNakedTuplesTest, NakedQuartet_1)
{
   CandidatesGrid grid;

   grid.setGiven(2, 1, 9);
   grid.setCandidates(Cell::rowCol2Field(0, 0), {1, 5});
   grid.setCandidates(Cell::rowCol2Field(0, 1), {1, 2, 4, 5});
   grid.setCandidates(Cell::rowCol2Field(0, 2), {2, 4, 5, 7});
   grid.setCandidates(Cell::rowCol2Field(1, 0), {1, 5, 6, 8});
   grid.setCandidates(Cell::rowCol2Field(1, 1), {1, 5, 6, 8});
   grid.setCandidates(Cell::rowCol2Field(1, 2), {3, 5, 6, 7, 8});
   grid.setCandidates(Cell::rowCol2Field(2, 0), {1, 6});
   grid.setCandidates(Cell::rowCol2Field(2, 2), {4, 6});

   size_t expectNumCandidates = 676;

   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   SweepNakedTuples strategy;
   FieldsInfo finfo(grid);
   // TestUtils::ShowStrategy(strategy, finfo);

   ASSERT_EQ(SweepNakedTuples::collectOfTupleSize(finfo, 2).size(), 0);
   ASSERT_EQ(SweepNakedTuples::collectOfTupleSize(finfo, 3).size(), 0);
   ASSERT_EQ(SweepNakedTuples::collectOfTupleSize(finfo, 4).size(), 7);

   TestUtils::CheckStrategy(strategy, grid, {7, 1});
}

TEST(SweepNakedTuplesTest, NakedQuartet_2)
{
   CandidatesGrid grid;

   grid.setCandidates(Cell::rowCol2Field(0, 0), {1, 2});
   grid.setCandidates(Cell::rowCol2Field(0, 3), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(0, 6), {3, 4});
   grid.setCandidates(Cell::rowCol2Field(0, 8), {1, 2, 4});
   grid.setCandidates(Cell::rowCol2Field(0, 2), {1, 2, 4, 5});

   SweepNakedTuples strategy;
   FieldsInfo finfo(grid);
   // TestUtils::ShowStrategy(strategy, finfo);

   ASSERT_EQ(SweepNakedTuples::collectOfTupleSize(finfo, 2).size(), 0);
   ASSERT_EQ(SweepNakedTuples::collectOfTupleSize(finfo, 3).size(), 0);
   ASSERT_EQ(SweepNakedTuples::collectOfTupleSize(finfo, 4).size(), 5 * 4 - 1);

   TestUtils::CheckStrategy(strategy, grid, {19, 1});
}
