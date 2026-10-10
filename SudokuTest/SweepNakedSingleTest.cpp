#include <gtest/gtest.h>

#include "Cell.h"
#include "Grid.h"
#include "House.h"
#include "SweepNakedSingle.h"
#include "TestModels.h"

using namespace Sudoku;

TEST(SweepNakedSingleTest, RowSimple)
{
   Grid grid;
   for (Field f = 1; f < NumIndex; ++f)
   {
      grid.setGiven(f, f + 1);
   }

   SweepNakedSingle().sweep(grid);
   ASSERT_EQ(grid.tryGetSolvedValue(0).value(), 1);
}

TEST(SweepNakedSingleTest, ColSimple)
{
   const Index row = 6;
   const Index col = 3;
   const Cell &cell = Cell::rowCol2Field(row, col);
   Grid grid;
   for (Index row = 0; row < NumIndex; ++row)
   {
      if (row != 3)
      {
         grid.setGiven(row, col, row + 1);
      }
   }

   SweepNakedSingle().sweep(grid);
   ASSERT_EQ(grid.tryGetSolvedValue(cell.field).value(), row + 1);
}

TEST(SweepNakedSingleTest, BoxSimple)
{
   const Index row = 6;
   const Index col = 3;
   const Cell &cell = Cell::rowCol2Field(row, col);
   const House &house = House::getBox(cell.box);
   const Field fieldToSkip = house.cells[3].field;
   Grid grid;
   Value val = 0;
   for (const Cell &c : house.cells)
   {
      if (c.field != fieldToSkip)
      {
         ++val;
         grid.setGiven(c.field, val);
      }
   }

   SweepNakedSingle().sweep(grid);
   ASSERT_EQ(grid.tryGetSolvedValue(fieldToSkip).value(), 9);
}

TEST(SweepNakedSingleTest, Diagonal)
{
   const Grid solved = TestModels::getSolved();
   Grid grid;
   for (const Cell &cell : Cell::getAll())
   {
      if (cell.row != cell.col)
      {
         grid.setGiven(cell.field, solved.tryGetSolvedValue(cell.field).value());
      }
   }

   SweepNakedSingle().sweep(grid);

   ASSERT_TRUE(grid.isSolved());
   ASSERT_NO_THROW(grid.checkValidSudoku());
}

TEST(SweepNakedSingleTest, SweepExample1)
{
   Grid grid;

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

   for (int s = 0; s < 10; ++s)
   {
      SweepNakedSingle().sweep(grid);
   }
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_EQ(grid.getNumCandidates(), 463);
}
