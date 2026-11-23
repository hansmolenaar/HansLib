#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "Cell.h"
#include "FieldsInfo.h"
#include "StrategyHiddenTuple.h"
#include "Sweep.h"
#include "TestUtils.h"

using namespace Sudoku;

TEST(SweepHidddenTupleTest, Strategy)
{
   SweepHiddenSingle single;
   TestUtils::CheckStrategyInterface(single);

   SweepHiddenTuple tuple;
   TestUtils::CheckStrategyInterface(tuple);
}

TEST(SweepHidddenTupleTest, Pair)
{
   CandidatesGrid grid;

   // Hidden pair 2, 6
   grid.setGiven(3, 1, 1);
   grid.setGiven(5, 1, 5);
   grid.setCandidates(Cell::getFromRowCol(3, 0).field, {3, 4, 7, 8, 9});
   grid.setCandidates(Cell::getFromRowCol(3, 2).field, {3, 4, 7, 9});

   grid.setCandidates(Cell::getFromRowCol(4, 0).field, {2, 3, 4, 6, 7, 8, 9});
   grid.setCandidates(Cell::getFromRowCol(4, 1).field, {3, 4, 7});
   grid.setCandidates(Cell::getFromRowCol(4, 2).field, {4, 7, 8, 9});
   grid.setCandidates(Cell::getFromRowCol(5, 0).field, {2, 3, 6, 7, 9});
   grid.setCandidates(Cell::getFromRowCol(5, 2).field, {4, 3, 7, 9});

   FieldsInfo finfo(grid);
   grid.update(finfo);

   Field expect = 658;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   FieldValue::Container fieldsValues;
   fieldsValues = SweepHiddenTuple::collectTuplesOfSize(finfo, 3);
   ASSERT_TRUE(fieldsValues.empty());
   fieldsValues = SweepHiddenTuple::collectTuplesOfSize(finfo, 3);
   ASSERT_TRUE(fieldsValues.empty());

   fieldsValues = SweepHiddenTuple::collectTuplesOfSize(finfo, 1);
   ASSERT_TRUE(fieldsValues.empty());

   fieldsValues = SweepHiddenTuple::collectTuplesOfSize(finfo, 2);
   TestUtils::CheckFieldValues(grid, fieldsValues);
   ASSERT_EQ(fieldsValues.size(), 8);

   SweepHiddenSingle strategy1;
   TestUtils::CheckStrategy(strategy1, grid, {0, 0});

   SweepHiddenTuple strategy;
   TestUtils::CheckStrategy(strategy, grid, {8, 0});
}

TEST(SweepHidddenTupleTest, SweepExample1)
{
   CandidatesGrid grid;

   grid.setGiven(0, 6, 5);
   grid.setGiven(1, 0, 1);
   grid.setGiven(1, 1, 6);
   grid.setGiven(1, 3, 9);
   grid.setGiven(2, 2, 9);
   grid.setGiven(2, 4, 6);
   grid.setGiven(2, 5, 4);
   grid.setGiven(3, 8, 4);
   grid.setGiven(4, 0, 4);
   grid.setGiven(4, 4, 2);
   grid.setGiven(4, 6, 1);
   grid.setGiven(5, 3, 3);
   grid.setGiven(5, 7, 5);
   grid.setGiven(6, 2, 2);
   grid.setGiven(6, 4, 8);
   grid.setGiven(6, 5, 9);
   grid.setGiven(7, 1, 1);
   grid.setGiven(7, 3, 2);
   grid.setGiven(7, 4, 5);
   grid.setGiven(7, 7, 3);
   grid.setGiven(8, 0, 7);
   grid.setGiven(8, 3, 1);
   grid.setGiven(8, 8, 9);

   FieldsInfo finfo(grid);
   grid.update(finfo);
   TestUtils::CheckFieldInfo(finfo);
   auto expect = grid.getNumCandidates();

   FieldValue::Container fieldsValues;
   fieldsValues = SweepHiddenTuple::collectTuplesOfSize(finfo, 1);
   TestUtils::CheckFieldValues(grid, fieldsValues);
   ASSERT_EQ(fieldsValues.size(), 16);

   auto changes = grid.apply(fieldsValues, finfo);
   TestUtils::CheckFieldInfo(finfo);
   ASSERT_EQ(changes.numValuesChanged, fieldsValues.size());
   ASSERT_EQ(changes.numFieldsSolved, 5);
   expect -= fieldsValues.size();
   ASSERT_EQ(grid.getNumCandidates(), expect);

   grid.update(finfo);
   TestUtils::CheckFieldInfo(finfo);
   SweepHiddenSingle strategy;
   Sweep(&strategy).singleSweep(grid, finfo);
   TestUtils::CheckFieldInfo(finfo);
   grid.update(finfo);
   TestUtils::CheckFieldInfo(finfo);
   ASSERT_EQ(grid.getNumCandidates(), 129);
}

TEST(SweepHidddenTupleTest, BasicExamplePair)
{
   constexpr Index row = 8;

   CandidatesGrid grid;
   grid.setGiven(row, 1, 6);
   grid.setGiven(row, 3, 4);
   grid.setGiven(row, 4, 2);
   grid.setGiven(row, 7, 8);
   grid.setGiven(row, 8, 9);

   grid.setCandidates(Cell::rowCol2Field(row, 0), {3, 5, 7});
   grid.setCandidates(Cell::rowCol2Field(row, 2), {1, 3});
   grid.setCandidates(Cell::rowCol2Field(row, 5), {1, 5});
   grid.setCandidates(Cell::rowCol2Field(row, 6), {1, 3, 5});

   SweepHiddenTuple strategy;
   // TestUtils::ShowStrategy(strategy, grid);
   TestUtils::CheckStrategy(strategy, grid, {0, 0});

   SweepHiddenSingle strategy1;

   auto value0 = grid[Cell::rowCol2Field(row, 0)].tryGetSolvedValue();
   ASSERT_FALSE(value0.has_value());

   FieldsInfo finfo(grid);
   const auto fieldValues = strategy1.collect(finfo);
   grid.apply(fieldValues, finfo);

   value0 = grid[Cell::rowCol2Field(row, 0)].tryGetSolvedValue();
   ASSERT_TRUE(value0.has_value());
   ASSERT_EQ(value0.value(), 7);
}

TEST(SweepHidddenTupleTest, Pair_1)
{
   constexpr Index row = 8;
   ValueBits pair;
   pair.set(8);
   pair.set(9);

   CandidatesGrid grid;

   for (Index col = 1; col < 8; ++col)
   {
      grid.removeCandidates(Cell::rowCol2Field(row, col), pair);
   }

   SweepHiddenTuple strategy;
   // TestUtils::ShowStrategy(strategy, grid);
   TestUtils::CheckStrategy(strategy, grid, {2 * 7, 0});
}

TEST(SweepHidddenTupleTest, Triple_322)
{
   constexpr Index row = 8;
   ValueBits bits;
   bits.set(7);
   bits.set(8);
   bits.set(9);

   CandidatesGrid grid;

   for (Index col = 1; col < 7; ++col)
   {
      grid.removeCandidates(Cell::rowCol2Field(row, col), bits);
   }
   bits.reset();
   bits.set(7);
   grid.removeCandidates(Cell::rowCol2Field(row, 0), bits);
   bits.reset();
   bits.set(9);
   grid.removeCandidates(Cell::rowCol2Field(row, 8), bits);

   SweepHiddenTuple strategy;
   FieldsInfo finfo(grid);
   const auto fieldValues = strategy.collectTuplesOfSize(finfo, 3);
   ASSERT_EQ(fieldValues.size(), 3 * 6);

   // TestUtils::ShowStrategy(strategy, grid);
   TestUtils::CheckStrategy(strategy, grid, {3 * 6, 0});
}

TEST(SweepHidddenTupleTest, Triple_332)
{
   constexpr Index row = 8;
   ValueBits bits;
   bits.set(7);
   bits.set(8);
   bits.set(9);

   CandidatesGrid grid;

   for (Index col = 1; col < 7; ++col)
   {
      grid.removeCandidates(Cell::rowCol2Field(row, col), bits);
   }
   bits.reset();
   bits.set(7);
   grid.removeCandidates(Cell::rowCol2Field(row, 8), bits);

   SweepHiddenTuple strategy;
   TestUtils::CheckStrategy(strategy, grid, {3 * 6, 0});
}

TEST(SweepHidddenTupleTest, Triple_222)
{
   constexpr Index row = 8;
   ValueBits bits;
   bits.set(7);
   bits.set(8);
   bits.set(9);

   CandidatesGrid grid;

   for (Index col = 1; col < 7; ++col)
   {
      grid.removeCandidates(Cell::rowCol2Field(row, col), bits);
   }
   bits.reset();
   bits.set(7);
   grid.removeCandidates(Cell::rowCol2Field(row, 0), bits);
   bits.reset();

   bits.reset();
   bits.set(8);
   grid.removeCandidates(Cell::rowCol2Field(row, 7), bits);

   bits.reset();
   bits.set(9);
   grid.removeCandidates(Cell::rowCol2Field(row, 8), bits);

   SweepHiddenTuple strategy;
   // TestUtils::ShowStrategy(strategy, grid);
   TestUtils::CheckStrategy(strategy, grid, {3 * 6, 0});
}

TEST(SweepHidddenTupleTest, Quartet_1)
{
   constexpr Index row = 0;
   ValueBits bits;
   bits.set(1);
   bits.set(7);
   bits.set(8);
   bits.set(9);

   CandidatesGrid grid;

   for (Index col = 2; col < 7; ++col)
   {
      grid.removeCandidates(Cell::rowCol2Field(row, col), bits);
   }
   bits.reset();
   bits.set(7);
   grid.removeCandidates(Cell::rowCol2Field(row, 0), bits);
   bits.reset();
   bits.set(9);
   grid.removeCandidates(Cell::rowCol2Field(row, 8), bits);

   SweepHiddenTuple strategy;
   // TestUtils::ShowStrategy(strategy, grid);
   TestUtils::CheckStrategy(strategy, grid, {4 * 5, 0});
}

TEST(SweepHidddenTupleTest, Quartet_2)
{
   CandidatesGrid grid;

   // 1 2 3 4 quartet
   ValueBits bits;
   bits.set(1);
   bits.set(2);
   bits.set(3);
   bits.set(4);
   grid.removeCandidates(Cell::rowCol2Field(0, 1), bits);
   grid.removeCandidates(Cell::rowCol2Field(1, 0), bits);
   grid.removeCandidates(Cell::rowCol2Field(1, 2), bits);
   grid.removeCandidates(Cell::rowCol2Field(2, 0), bits);
   grid.removeCandidates(Cell::rowCol2Field(2, 1), bits);

   grid.setCandidates(Cell::rowCol2Field(0, 0), {1, 2, 5, 6, 7, 8});
   grid.setCandidates(Cell::rowCol2Field(0, 2), {1, 3, 5, 6, 7, 8});
   grid.setCandidates(Cell::rowCol2Field(1, 1), {2, 3, 5, 6, 7, 8});
   grid.setCandidates(Cell::rowCol2Field(2, 2), {3, 4, 5, 6, 7, 8});

   SweepHiddenTuple strategy;
   // TestUtils::ShowStrategy(strategy, grid);
   TestUtils::CheckStrategy(strategy, grid, {4 * 4, 0});
}
