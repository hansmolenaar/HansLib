#include <gtest/gtest.h>

#include "SolverSweep.h"
#include "FieldInfoStatic.h"
#include "SubSetPotentialsSweep.h"
#include "Defines.h"

#include <unordered_map>

using namespace Sudoku;

TEST(SubSetPotentialsSweepTest, SweepSingle)
{
   constexpr FieldIndex FieldToUse = 5;
   constexpr Value ValueActive = 6;
   Potentials potentials;
   potentials.setSingle(FieldToUse, ValueActive);
   auto subset = potentials.getSubSetPotentials(RowColBoxType::Row, 0);
   SubSetPotentialsSweepSingles sweep;
   const bool changed = sweep(subset);
   ASSERT_TRUE(changed);
   ASSERT_EQ(potentials.getSingleOrUndefined(FieldToUse), ValueActive);
   for (auto item : RowColBoxPositionsAll)
   {
      if (item != FieldToUse)
      {
         ASSERT_EQ(potentials.get(item).count(), 8);
      }
   }
}

TEST(SubSetPotentialsSweepTest, SweepDigons)
{
   SubSetPotentialsSweepClusters sweep;
   std::array<Potential, NumRowColBoxPositions> potentialsMem;

   SubSetPotentials potentials;
   str::transform(potentialsMem, potentials.begin(), [](auto& p) {return &p;  });

   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(0) = Potential::Create({ 1, 9 });
   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(2) = Potential::Create({ 1, 4 });
   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(8) = Potential::Create({ 9, 1 });
   ASSERT_TRUE(sweep(potentials));

   ASSERT_EQ(potentials.at(0)->count(), 2);
   ASSERT_EQ(potentials.at(2)->count(), 1);
   ASSERT_EQ(potentials.at(3)->count(), 7);
   ASSERT_EQ(potentials.at(8)->count(), 2);
}


TEST(SubSetPotentialsSweepTest, Sweep223)
{
   SubSetPotentialsSweepClusters sweep;
   std::array<Potential, NumRowColBoxPositions> potentialsMem;

   SubSetPotentials potentials;
   str::transform(potentialsMem, potentials.begin(), [](auto& p) {return &p;  });

   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(0) = Potential::Create({ 1, 9 });
   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(2) = Potential::Create({ 1, 4 });
   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(8) = Potential::Create({ 4, 5 });
   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(6) = Potential::Create({ 9, 1, 4 });
   ASSERT_TRUE(sweep(potentials));

   ASSERT_EQ(potentials.at(0)->count(), 2);
   ASSERT_EQ(potentials.at(2)->count(), 2);
   ASSERT_EQ(potentials.at(3)->count(), 6);
   ASSERT_EQ(potentials.at(6)->count(), 3);
   ASSERT_EQ(potentials.at(7)->count(), 6);
   ASSERT_EQ(potentials.at(8)->count(), 1);
}


TEST(SubSetPotentialsSweepTest, Sweep2234)
{
   SubSetPotentialsSweepClusters sweep;
   std::array<Potential, NumRowColBoxPositions> potentialsMem;

   SubSetPotentials potentials;
   str::transform(potentialsMem, potentials.begin(), [](auto& p) {return &p;  });

   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(0) = Potential::Create({ 1,2, 8, 9 });
   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(1) = Potential::Create({ 2, 4, 8, 9 });
   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(2) = Potential::Create({ 2, 4, 8 });
   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(3) = Potential::Create({ 4, 9 });
   ASSERT_FALSE(sweep(potentials));

   potentialsMem.at(5) = Potential::Create({ 2, 8 });
   ASSERT_TRUE(sweep(potentials));

   ASSERT_EQ(potentials.at(0)->count(), 1);
   ASSERT_EQ(potentials.at(1)->count(), 4);
   ASSERT_EQ(potentials.at(2)->count(), 3);
   ASSERT_EQ(potentials.at(3)->count(), 2);
   ASSERT_EQ(potentials.at(5)->count(), 2);
   ASSERT_EQ(potentials.at(7)->count(), 5);
   ASSERT_EQ(potentials.at(8)->count(), 5);
}