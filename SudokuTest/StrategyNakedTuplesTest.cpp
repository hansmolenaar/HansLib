#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "Cell.h"
#include "FieldsInfo.h"
#include "House.h"
#include "StrategyNakedTuples.h"
#include "SudokuDefines.h"
#include "Sweep.h"
#include "TestUtils.h"

#include <iostream>

using namespace Sudoku;

namespace
{

FieldValues CollectTupleSize(const FieldsInfo &finfo, Index size)
{
   FieldValues result;
   StrategyNakedTuples::collectOfTupleSize(finfo, size, result);
   return result;
}

FieldValues CollectHousesOfType(const FieldsInfo &finfo, HouseType houseType, Index size)
{
   FieldValues result;
   StrategyNakedTuples::collectHousesOfType(finfo, houseType, size, result);
   return result;
}

} // namespace

TEST(StrategyNakedTuplesTest, Strategy)
{
   StrategyNakedTuples strategy;
   TestUtils::CheckStrategyInterface(strategy);
}

TEST(StrategyNakedTuplesTest, Pair_1)
{
   StrategyNakedTuples strategy;
   Sweep sweep(&strategy);
   CandidatesGrid grid;

   grid.setCandidates(Cell::rowCol2Field(0, 0), {1, 2});
   grid.setCandidates(Cell::rowCol2Field(0, 1), {1, 2});

   TestUtils::CheckStrategy(strategy, grid, GridChange{26, 0});
}

TEST(StrategyNakedTuplesTest, NakedPair_2)
{
   StrategyNakedTuples strategy;
   Sweep sweep(&strategy);
   CandidatesGrid grid;
   grid.setCandidates(Cell::rowCol2Field(0, 0), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(1, 1), {3, 2});

   const FieldsInfo finfo(grid);
   TestUtils::CheckStrategy(strategy, grid, GridChange{14, 0});
}

TEST(StrategyNakedTuplesTest, StrategyNakedPair_2)
{
   StrategyNakedTuples strategy;
   Sweep sweep(&strategy);
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

   const FieldsInfo finfo(grid);
   // TestUtils::ShowStrategy(strategy, grid);

   auto fieldValues = CollectHousesOfType(finfo, HouseType::Col, 2);
   ASSERT_EQ(fieldValues.size(), 0);

   fieldValues = CollectHousesOfType(finfo, HouseType::Box, 2);
   ASSERT_EQ(fieldValues.size(), 0);

   fieldValues = CollectHousesOfType(finfo, HouseType::Row, 2);
   ASSERT_EQ(fieldValues.size(), 2);

   fieldValues = CollectTupleSize(finfo, 2); // 1 5
   ASSERT_EQ(fieldValues.size(), 2);

   fieldValues = CollectTupleSize(finfo, 3); // 1 5 6
   ASSERT_EQ(fieldValues.size(), 3);

   TestUtils::CheckStrategy(strategy, grid, {4, 1});
}

TEST(StrategyNakedTuplesTest, StrategyNakedTuples_1)
{
   const std::array<Value, NumField> diagramValues = {
       4, 0, 0, 0, 0, 0, 9, 3, 8, 0, 3, 2, 0, 9, 4, 1, 0, 0, 0, 9, 5, 3, 0, 0, 2, 4, 0,
       3, 7, 0, 6, 0, 9, 0, 0, 4, 5, 2, 9, 0, 0, 1, 6, 7, 3, 6, 0, 4, 7, 0, 3, 0, 9, 0,
       9, 5, 7, 0, 0, 8, 3, 0, 0, 0, 0, 3, 9, 0, 0, 4, 0, 0, 2, 4, 0, 0, 3, 0, 7, 0, 9,
   };

   StrategyNakedTuples strategy;
   auto grid = CandidatesGrid::FromValues(diagramValues);
   FieldsInfo finfo(grid);
   grid.update(finfo);
   Field expect = 152;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   auto fieldValues = CollectHousesOfType(finfo, HouseType::Col, 3);
   ASSERT_TRUE(fieldValues.get().empty());

   auto changes = grid.apply(fieldValues, finfo);
   ASSERT_EQ(changes.numValuesChanged, 0);

   fieldValues = CollectHousesOfType(finfo, HouseType::Row, 3);
   ASSERT_EQ(fieldValues.size(), 0);

   fieldValues = CollectHousesOfType(finfo, HouseType::Box, 3);
   ASSERT_EQ(grid.getNumCandidates(), expect);

   fieldValues = CollectHousesOfType(finfo, HouseType::Col, 2);
   ASSERT_EQ(grid.getNumCandidates(), expect);

   TestUtils::CheckStrategy(strategy, grid, {14, 2});
}

TEST(StrategyNakedTuplesTest, Box_222)
{
   CandidatesGrid grid;

   grid.setCandidates(Cell::rowCol2Field(6, 6), {1, 2});
   grid.setCandidates(Cell::rowCol2Field(7, 7), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(8, 8), {3, 1});

   size_t expectNumCandidates = NumField * NumValue - 3 * 7;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   StrategyNakedTuples strategy;
   FieldsInfo finfo(grid);
   auto fieldValues = CollectHousesOfType(finfo, HouseType::Col, 3);
   ASSERT_TRUE(fieldValues.get().empty());

   fieldValues = CollectHousesOfType(finfo, HouseType::Row, 3);
   ASSERT_TRUE(fieldValues.get().empty());

   fieldValues = CollectHousesOfType(finfo, HouseType::Box, 3);
   ASSERT_EQ(fieldValues.size(), 3 * 6);

   TestUtils::CheckStrategy(strategy, grid, {18, 0});
}

TEST(StrategyNakedTuplesTest, Row_222)
{
   CandidatesGrid grid;

   const Index row = 7;
   grid.setCandidates(Cell::rowCol2Field(row, 2), {1, 2});
   grid.setCandidates(Cell::rowCol2Field(row, 8), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(row, 5), {3, 1});

   size_t expectNumCandidates = NumField * NumValue - 3 * 7;
   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   StrategyNakedTuples strategy;
   FieldsInfo finfo(grid);
   auto fieldValues = CollectHousesOfType(finfo, HouseType::Col, 3);
   ASSERT_TRUE(fieldValues.get().empty());

   fieldValues = CollectHousesOfType(finfo, HouseType::Box, 3);
   ASSERT_TRUE(fieldValues.get().empty());

   fieldValues = CollectHousesOfType(finfo, HouseType::Row, 3);
   ASSERT_EQ(fieldValues.size(), 3 * 6);

   TestUtils::CheckStrategy(strategy, grid, {18, 0});
}

TEST(StrategyNakedTuplesTest, StrategyNakedTuples_223_a)
{
   CandidatesGrid grid;

   grid.setCandidates(Cell::rowCol2Field(6, 6), {4, 5});
   grid.setCandidates(Cell::rowCol2Field(7, 7), {5, 6});
   grid.setCandidates(Cell::rowCol2Field(8, 8), {6, 4, 5});

   size_t expectNumCandidates = NumField * NumValue - 2 * 7 - 6;

   ASSERT_EQ(grid.getNumCandidates(), expectNumCandidates);

   StrategyNakedTuples strategy;
   const FieldsInfo finfo(grid);
   // TestUtils::ShowStrategy(strategy, grid);

   auto fieldValues = CollectHousesOfType(finfo, HouseType::Col, 3);
   ASSERT_TRUE(fieldValues.get().empty());

   fieldValues = CollectHousesOfType(finfo, HouseType::Row, 3);
   ASSERT_TRUE(fieldValues.get().empty());

   fieldValues = CollectHousesOfType(finfo, HouseType::Box, 3);
   ASSERT_EQ(fieldValues.size(), 3 * 6);

   TestUtils::CheckStrategy(strategy, grid, {18, 0});
}

TEST(StrategyNakedTuplesTest, NakedQuartet_1)
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

   StrategyNakedTuples strategy;
   const FieldsInfo finfo(grid);
   // TestUtils::ShowStrategy(strategy, grid);

   ASSERT_EQ(CollectTupleSize(finfo, 2).size(), 0);
   ASSERT_EQ(CollectTupleSize(finfo, 3).size(), 0);
   ASSERT_EQ(CollectTupleSize(finfo, 4).size(), 7);

   TestUtils::CheckStrategy(strategy, grid, {7, 1});
}

TEST(StrategyNakedTuplesTest, NakedQuartet_2)
{
   CandidatesGrid grid;

   grid.setCandidates(Cell::rowCol2Field(0, 0), {1, 2});
   grid.setCandidates(Cell::rowCol2Field(0, 3), {2, 3});
   grid.setCandidates(Cell::rowCol2Field(0, 6), {3, 4});
   grid.setCandidates(Cell::rowCol2Field(0, 8), {1, 2, 4});
   grid.setCandidates(Cell::rowCol2Field(0, 2), {1, 2, 4, 5});

   StrategyNakedTuples strategy;
   const FieldsInfo finfo(grid);
   // TestUtils::ShowStrategy(strategy, grid);

   ASSERT_EQ(CollectTupleSize(finfo, 2).size(), 0);
   ASSERT_EQ(CollectTupleSize(finfo, 3).size(), 0);
   ASSERT_EQ(CollectTupleSize(finfo, 4).size(), 5 * 4 - 1);

   TestUtils::CheckStrategy(strategy, grid, {19, 1});
}

TEST(StrategyNakedTuplesTest, NakedBendedTriplet_1)
{
   CandidatesGrid grid;
   grid.setCandidates(Cell::rowCol2Field(6, 5), {6, 9});
   grid.setCandidates(Cell::rowCol2Field(7, 4), {5, 6, 9});
   grid.setCandidates(Cell::rowCol2Field(8, 4), {5, 6, 9});

   StrategyNakedTuples strategy;
   // TestUtils::ShowStrategy(strategy, grid);
   TestUtils::CheckStrategy(strategy, grid, {6 * 3 + 6, 0});
}
