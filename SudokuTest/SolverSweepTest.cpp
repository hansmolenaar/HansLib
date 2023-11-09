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
   auto potentials = diagram.getPotentialS();
   const bool changed = SolverSweepRow()(potentials);
   ASSERT_TRUE(changed);
   ASSERT_EQ(potentials.GetSingleOrUndefined(FieldToUse), ValueActive);
}
