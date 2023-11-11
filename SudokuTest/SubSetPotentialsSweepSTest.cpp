#include <gtest/gtest.h>

#include "SolverSweep.h"
#include "FieldInfoStatic.h"
#include "SubSetPotentialsSweep.h"

#include <unordered_map>

using namespace Sudoku;

TEST(SubSetPotentialsSweepTest, SweepSingle)
{
   constexpr FieldIndex FieldToUse = 5;
   constexpr Value ValueActive = 6;
   Potentials potentials;
   potentials.setSingle(FieldToUse, ValueActive);
   auto subset = potentials.getSubSetPotentials(SubSetType::Row, 0);
   SubSetPotentialsSweepSingles sweep;
   const bool changed = sweep(subset);
   ASSERT_TRUE(changed);
   ASSERT_EQ(potentials.getSingleOrUndefined(FieldToUse), ValueActive);
   for (auto item : SubSetItemsAll)
   {
      if (item != FieldToUse)
      {
         ASSERT_EQ(potentials.get(item).count(), 8);
      }
   }
}