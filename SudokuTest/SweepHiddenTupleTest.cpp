#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "Cell.h"
#include "FieldsInfo.h"
#include "SweepHiddenTuple.h"
#include "TestUtils.h"

#include "StreamUtils.h"
#include <iostream>

using namespace Sudoku;

TEST(SweepHidddenTupleTest, BasicExample)
{
   CandidatesGrid grid;
   FieldsInfo finfo(grid);
   for (Index size = 1; size <= 3; ++size)
   {
      const auto fieldValues = SweepHiddenTuple::collectTuplesOfSize(finfo, size);
      ASSERT_TRUE(fieldValues.empty());
   }
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
   finfo.update();
   auto changes = grid.apply(fieldsValues, finfo);
   ASSERT_EQ(changes.numValuesChanged, 8);
   ASSERT_EQ(changes.numFieldsSolved, 0);
   expect -= changes.numValuesChanged;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   grid.update(finfo);
   ASSERT_EQ(grid.getNumCandidates(), expect);
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
   SweepHiddenSingle().sweep(grid);
   finfo.update();
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

   auto value0 = grid[Cell::rowCol2Field(row, 0)].tryGetSolvedValue();
   ASSERT_FALSE(value0.has_value());

   // CandidatesGrid prv;
   // prv = grid;
   // std::cout << prv << "\n";
   SweepHiddenSingle().sweep(grid);
   // std::cout << grid << "\n";
   // std::cout << CandidatesGrid::DifferenceToStrings(prv, grid);

   value0 = grid[Cell::rowCol2Field(row, 0)].tryGetSolvedValue();
   ASSERT_TRUE(value0.has_value());
   ASSERT_EQ(value0.value(), 7);
}
