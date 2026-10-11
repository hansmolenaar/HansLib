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
      grid.setValue(f, f + 1);
   }

   SweepNakedSingle().sweep(grid.get());
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
         grid.setValue(row, col, row + 1);
      }
   }

   SweepNakedSingle().sweep(grid.get());
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
         grid.setValue(c.field, val);
      }
   }

   SweepNakedSingle().sweep(grid.get());
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
         grid.setValue(cell.field, solved.tryGetSolvedValue(cell.field).value());
      }
   }

   SweepNakedSingle().sweep(grid.get());

   ASSERT_TRUE(grid.isSolved());
   ASSERT_NO_THROW(grid.checkValidSudoku());
}

TEST(SweepNakedSingleTest, SweepExample1)
{
   Grid grid;

   grid.setValue(0, 0, 2);
   grid.setValue(0, 1, 5);
   grid.setValue(0, 2, 8);
   grid.setValue(1, 7, 7);
   grid.setValue(2, 0, 3);
   grid.setValue(2, 2, 9);
   grid.setValue(2, 4, 4);
   grid.setValue(4, 1, 6);
   grid.setValue(6, 1, 4);
   grid.setValue(6, 2, 6);

   for (int s = 0; s < 10; ++s)
   {
      SweepNakedSingle().sweep(grid.get());
   }
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_EQ(grid.getNumCandidates(), 463);
}
