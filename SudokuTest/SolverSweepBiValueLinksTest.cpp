#include <gtest/gtest.h>

#include "SolverSweepBiValueLinks.h"
#include "TestModels.h"

using namespace Sudoku;

TEST(SolverSweepBiValueLinksTest, GetBiValueFields)
{
   const Diagram diagram = TestModels::getBiValue1();
   auto potentials = diagram.getPotentials();

   // Set the potentials
   SolverSweepTrivial sweep;
   const auto status = sweep(potentials);

   const auto start = potentials.toString();
   ASSERT_EQ(potentials.getNumSingles(), 53);
   const auto biValueFields = SolverSweepBiValueLinks::GetBiValueFields(potentials);
   ASSERT_EQ(biValueFields.size(), 19);

}
