#include <gtest/gtest.h>

#include "FieldsInfo.h"
#include "Logging.h"
#include "Strategies.h"
#include "Sweep.h"
#include "TestModels.h"

using namespace Sudoku;
using namespace Utilities;

TEST(StrategiesTest, Simple)
{
   Sweep sweep(Strategies::getSimpleStrategies());
   auto grid = TestModels::getSimplePuzzles().at(10);
   FieldsInfo finfo(grid);
   grid.update(finfo);
   Logging::setDoLog(true);
   sweep.iterate(grid, finfo);
   Logging::setDoLog(false);
   ASSERT_TRUE(grid.isSolved());
}
