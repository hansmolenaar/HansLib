#include <gtest/gtest.h>

#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "Cell.h"
#include "FieldsInfo.h"
#include "House.h"
#include "SweepNakedSingle.h"
#include "TestUtils.h"

using namespace Sudoku;
using namespace Utilities;

TEST(SweepNakedSingleTest, Strategy)
{
   SweepNakedSingle strategy;
   TestUtils::CheckStrategy(strategy);
}

TEST(SweepNakedSingleTest, RowSimple)
{
   CandidatesGrid grid;
   for (Field f = 1; f < NumIndex; ++f)
   {
      const Value value = f + 1;
      grid.setCandidates(f, {value});
   }

   FieldsInfo finfo(grid);
   const auto collected = SweepNakedSingle().collect(finfo);
   ASSERT_TRUE(collected.contains({0, 8}));
   ASSERT_EQ(collected.size(), 104);

   SweepNakedSingle().sweep(grid, finfo);
   TestUtils::CheckFieldInfo(finfo);
   ASSERT_EQ(grid[0].tryGetSolvedValue().value(), 1);
}

TEST(SweepNakedSingleTest, ColSimple)
{
   const Index row = 6;
   const Index col = 3;
   const Cell cell(Cell::rowCol2Field(row, col));
   CandidatesGrid grid;
   for (Index row = 0; row < NumIndex; ++row)
   {
      if (row != 3)
      {
         grid.setGiven(row, col, row + 1);
      }
   }

   FieldsInfo finfo(grid);
   SweepNakedSingle().sweep(grid, finfo);
   ASSERT_EQ(grid[cell.field].tryGetSolvedValue().value(), row + 1);
}

TEST(SweepNakedSingleTest, BoxSimple)
{
   const Index row = 6;
   const Index col = 3;
   CandidatesGrid grid;
   FieldsInfo finfo(grid);
   const Cell cell(Cell::rowCol2Field(row, col));
   const auto house = finfo.getHouseFields(House{HouseType::Box, cell.box});
   const Field fieldToSkip = *(FieldAll | std::views::filter(Bitset{house}) | std::views::drop(2)).begin();
   Value val = 0;
   for (Field field : FieldAll | std::views::filter(Bitset{house}))
   {
      if (field != fieldToSkip)
      {
         ++val;
         grid.setGiven(field, val);
      }
   }

   finfo.update();
   SweepNakedSingle().sweep(grid, finfo);
   ASSERT_EQ(grid[fieldToSkip].tryGetSolvedValue().value(), 9);
}

TEST(SweepNakedSingleTest, SweepExample1)
{
   CandidatesGrid grid;

   grid.setGiven(0, 0, 2);
   grid.setGiven(0, 1, 5);
   grid.setGiven(0, 2, 8);
   grid.setGiven(1, 7, 7);
   grid.setGiven(2, 0, 3);
   grid.setGiven(2, 2, 9);
   grid.setGiven(2, 4, 4);
   grid.setGiven(4, 1, 6);
   grid.setGiven(6, 1, 4);
   grid.setGiven(6, 2, 6);

   FieldsInfo finfo(grid);
   for (int s = 0; s < 10; ++s)
   {
      SweepNakedSingle().sweep(grid, finfo);
      TestUtils::CheckFieldInfo(finfo);
   }
   ASSERT_EQ(grid.getNumCandidates(), 463);
}
