#include <gtest/gtest.h>

#include "Potentials.h"
#include "FieldInfoStatic.h"

using namespace Sudoku;

TEST(PotentialsTest, ToString)
{
   Potentials potentials;
   for (auto f : FieldInfoStatic::getAllFields())
   {
      potentials.setSingle(f, (f % 9) + 1);
   }
   ASSERT_EQ(potentials.getNumSingles(), NumFields);
   const auto str = potentials.toString();
   ASSERT_FALSE(str.empty());
}