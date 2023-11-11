#include <gtest/gtest.h>

#include "Potentials.h"

using namespace Sudoku;

TEST(PotentialsTest, ToString)
{
   Potentials potentials;
   for (auto f = 0; f < NumFields; ++f)
   {
      potentials.setSingle(f, (f % 9) + 1);
   }
   const auto str = potentials.toString();
   ASSERT_FALSE(str.empty());
}