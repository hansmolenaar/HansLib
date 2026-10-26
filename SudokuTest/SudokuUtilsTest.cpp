#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "House.h"
#include "Single.h"
#include "SudokuUtils.h"

#include <algorithm>

using namespace Sudoku;

TEST(SudokuUtilsTest, Basic)
{
   CandidatesGrid grid;
   grid.setGiven(1, 1);
   grid.setGiven(2, 2);
   std::vector<Field> someFields = {0, 2};

   auto retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someFields);
   ASSERT_EQ(retval.size(), 8);

   grid.update();
   retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someFields);
   ASSERT_EQ(retval.size(), 7);

   someFields = {1, 2};
   retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someFields);
   ASSERT_EQ(retval.size(), 0);

   someFields = {0, 4};
   retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someFields);
   ASSERT_EQ(retval.size(), 7);
}

TEST(SudokuUtilsTest, GetActiveCandidatesInCells)
{
   CandidatesGrid cg;
   const auto house = House::getRow(0);
   auto retval = SudokuUtils::getUniqueActiveValuesInCells(cg, house.cells);
   ASSERT_EQ(retval.size(), NumCellsInHouse);

   cg.removeCandidate(0, 9);
   retval = SudokuUtils::getUniqueActiveValuesInCells(cg, house.cells);
   ASSERT_EQ(retval.size(), NumCellsInHouse);

   cg.setGiven(1, 5);
   retval = SudokuUtils::getUniqueActiveValuesInCells(cg, house.cells);
   ASSERT_EQ(retval.size(), NumCellsInHouse - 1);
}

TEST(SudokuUtilsTest, CountOccurence)
{
   constexpr Field field = 2;
   CandidatesGrid cg;
   const auto house = House::getRow(0);

   cg.setCandidates(field, {3, 4});
   ASSERT_EQ(SudokuUtils::countOccurence(cg, house.cells, 2), NumCellsInHouse - 1);
   ASSERT_EQ(SudokuUtils::countOccurence(cg, house.cells, 3), NumCellsInHouse);
}

TEST(SudokuUtilsTest, GetSolvedCells)
{
   CandidatesGrid grid;
   const auto house = House::getBox(8);
   auto retval = SudokuUtils::getNotSolvedCells(grid, house);
   ASSERT_EQ(retval.size(), NumCellsInHouse);

   grid.setGiven(6, 6, 1);
   retval = SudokuUtils::getNotSolvedCells(grid, house);
   ASSERT_EQ(retval.size(), NumCellsInHouse - 1);

   grid.setGiven(7, 7, 5);
   retval = SudokuUtils::getNotSolvedCells(grid, house);
   ASSERT_EQ(retval.size(), NumCellsInHouse - 2);

   const std::vector<Field> expect{61, 62, 69, 71, 78, 79, 80};
   ASSERT_TRUE(std::equal(retval.begin(), retval.end(), expect.begin(), expect.end(), Cell::AreSame));
}

TEST(SudokuUtilsTest, GetValue2Fields)
{
   CandidatesGrid grid;
   grid.setGiven(0, 1);
   grid.setGiven(8, 8);
   grid.removeCandidate(Cell::rowCol2Field(0, 5), 5);

   std::vector<Field> fields{2};

   auto v2f = SudokuUtils::getValue2Fields(grid, std::span(fields));
   ASSERT_EQ(Utilities::Single(v2f[1]), 2);
   ASSERT_EQ(Utilities::Single(v2f[2]), 2);
   ASSERT_EQ(Utilities::Single(v2f[8]), 2);

   fields.push_back(0);
   v2f = SudokuUtils::getValue2Fields(grid, std::span(fields));
   ASSERT_EQ(v2f[1].size(), 2);
   ASSERT_EQ(v2f[2].size(), 1);
   ASSERT_EQ(v2f[8].size(), 1);

   fields.push_back(8);
   v2f = SudokuUtils::getValue2Fields(grid, std::span(fields));
   ASSERT_EQ(v2f[1].size(), 2);
   ASSERT_EQ(Utilities::Single(v2f[2]), 2);
   ASSERT_EQ(v2f[8].size(), 2);
}
