#include <gtest/gtest.h>

#include "SolverSweep.h"
#include "FieldInfoStatic.h"
#include "Diagram.h"

#include <unordered_map>

using namespace Sudoku;

TEST(SolverSweepRowTest, GetPotentialsCol)
{
   constexpr RowColIndex ColToSkip = 8;
   constexpr Value ValueActive = ColToSkip + 1;
   constexpr RowColIndex RowToUse = 0;
   constexpr FieldIndex FieldToUse = FieldInfoStatic::RowColToField(RowToUse, ColToSkip);

   std::unordered_map<FieldIndex, Value> input;
   for (auto col : RowColAll)
   {
      if (col == ColToSkip) continue;
      input[FieldInfoStatic::RowColToField(RowToUse, col)] = col + 1;
   }

   const Diagram diagram = Diagram::Create(input);
   auto potentials = diagram.getPotentials();
   SolverSweepRow sweep;
   const bool changed = sweep(potentials);
   ASSERT_TRUE(changed);
   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldToUse), ValueActive);
}


TEST(SolverSweepRowTest, GetPotentialsRow)
{
   constexpr RowColIndex RowToSkip = 6;
   constexpr Value ValueActive = RowToSkip + 1;
   constexpr RowColIndex ColToUse = 5;
   constexpr FieldIndex FieldToUse = FieldInfoStatic::RowColToField(RowToSkip, ColToUse);

   std::unordered_map<FieldIndex, Value> input;
   for (auto row : RowColAll)
   {
      if (row == RowToSkip) continue;
      input[FieldInfoStatic::RowColToField(row, ColToUse)] = row + 1;
   }

   const Diagram diagram = Diagram::Create(input);
   auto potentials = diagram.getPotentials();
   SolverSweepColumns sweep;
   const bool changed = sweep(potentials);
   ASSERT_TRUE(changed);
   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldToUse), ValueActive);
}


TEST(SolverSweepRowTest, GetPotentialsSubSquare)
{
   const std::array<FieldInfoStatic, NumFields>& instance = FieldInfoStatic::Instance();
   constexpr RowColIndex FieldToSkip = 79;

   std::unordered_map<FieldIndex, Value> input;
   Value value = 1;
   for (RowColIndex row = 6; row < NumRowCol; ++row)
   {
      for (RowColIndex col = 6; col < NumRowCol; ++col)
      {
         const FieldIndex field = FieldInfoStatic::RowColToField(row, col);
         if (field == FieldToSkip) continue;
         input[field] = value;
         ++value;
      }
   }

   const Diagram diagram = Diagram::Create(input);
   auto potentials = diagram.getPotentials();
   SolverSweepSubSquares sweep;
   const bool changed = sweep(potentials);
   ASSERT_TRUE(changed);
   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldToSkip), value);
}