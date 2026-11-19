#include <gtest/gtest.h>

#include "TestUtils.h"

using namespace Sudoku;

void TestUtils::CheckFieldValues(const CandidatesGrid &grid, const FieldValue::Container &fieldsValues)
{
   for (auto fv : fieldsValues)
   {
      ASSERT_TRUE(grid[fv.field].contains(fv.value));
   }
}
