#include <gtest/gtest.h>

#include "FieldsInfo.h"
#include "IStrategy.h"
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
   // Logging::setDoLog(true);
   sweep.iterate(grid, finfo);
   // Logging::setDoLog(false);
   ASSERT_TRUE(grid.isSolved());
}

TEST(StrategiesTest, Hard)
{
   const std::string puzzle = "1.85..4.6....7.9..53...4..7..1.6...8.9.4.8.7.8...5.6..7..1...69..6.8....9.4..62.5";
   auto allStrategies = Strategies::getAllStrategies();
#if false
   for (const auto *s : allStrategies.get())
   {
      std::cout << s->getName() << "\n";
   }
#endif
   Sweep sweep(std::move(allStrategies));
   auto grid = CandidatesGrid::FromString(puzzle);
   FieldsInfo finfo(grid);
   grid.update(finfo);
   // Logging::setDoLog(true);
   sweep.iterate(grid, finfo);
   // Logging::setDoLog(false);
   ASSERT_TRUE(grid.isSolved());
}
