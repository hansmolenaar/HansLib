#include <gtest/gtest.h>

#include "SolverSweepBiValueLinks.h"
#include "TestModels.h"
#include "Defines.h"

using namespace Sudoku;

TEST(SolverSweepBiValueLinksTest, GetBiValueFields)
{
   const Diagram diagram = TestModels::getBiValue1();
   auto potentials = diagram.getPotentials();
   SolverSweepTrivial sweep;
   const auto status = sweep(potentials);
   const auto start = potentials.toString();
   ASSERT_EQ(potentials.getNumSingles(), 53);

   const auto biValueFields = SolverSweepBiValueLinks::GetBiValueFields(potentials, 4);
   ASSERT_EQ(biValueFields.size(), 9);
}


TEST(SolverSweepBiValueLinksTest, GetBiValueAdjecencies)
{
   const Diagram diagram = TestModels::getBiValue1();
   auto potentials = diagram.getPotentials();
   SolverSweepTrivial sweep;
   sweep(potentials);

   const auto adjacencies = SolverSweepBiValueLinks::GetBiValueAdjecencies(potentials, 7);
   ASSERT_EQ(adjacencies.size(), 3);
   ASSERT_EQ(2, str::count_if(adjacencies, [](const auto p) {return p.first == 8 || p.second == 8; }));
}
