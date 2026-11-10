#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "SweepSimpleColoring.h"

#include <iostream>

using namespace Sudoku;

namespace
{
CandidatesGrid getSimpleExample()
{
   CandidatesGrid result;
   for (Cell cell : Cell::getAll())
   {
      const bool bot_top = cell.row == 0 || cell.row == 8;
      const bool left_right = cell.col == 0 || cell.col == 8;
      if (bot_top != left_right)
      {
         result.removeCandidate(cell.field, 1);
      }
   }

   return result;
}

}; // namespace

TEST(SweepSimpleColoringTest, BasicExample)
{
   const CandidatesGrid grid = getSimpleExample();
   std::cout << grid;
   const auto links = SweepSimpleColoring::getBilocationLinks(grid, 1);
   ASSERT_EQ(links.size(), 4);
}
