#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "StreamUtils.h"
#include "SweepIntersectionElimination.h"

using namespace Sudoku;

TEST(SweepIntersectionElimination, BoxRow)
{
   std::vector<Cell> cells{Cell::get(0), Cell::get(2)};
   std::optional<HouseId> retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, cells);
   ASSERT_TRUE(retval.has_value());
   ASSERT_EQ(retval.value(), (HouseId{HouseType::Row, 0}));

   cells.push_back(Cell::get(1));
   retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, cells);
   ASSERT_TRUE(retval.has_value());
   ASSERT_EQ(retval.value(), (HouseId{HouseType::Row, 0}));

   cells.push_back(Cell::get(3));
   ASSERT_ANY_THROW(SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, cells));

   cells = {Cell::get(0), Cell::get(1), Cell::get(11)};
   retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, cells);
   ASSERT_FALSE(retval.has_value());

   cells = {Cell::get(0), Cell::get(10)};
   retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, cells);
   ASSERT_FALSE(retval.has_value());
}

TEST(SweepIntersectionElimination, BoxCol)
{
   std::vector<Cell> cells{Cell::getFromRowCol(0, 5), Cell::getFromRowCol(2, 5)};
   std::optional<HouseId> retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, cells);
   ASSERT_TRUE(retval.has_value());
   ASSERT_EQ(retval.value(), (HouseId{HouseType::Col, 5}));

   cells.push_back(Cell::getFromRowCol(1, 5));
   retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, cells);
   ASSERT_TRUE(retval.has_value());
   ASSERT_EQ(retval.value(), (HouseId{HouseType::Col, 5}));

   cells.push_back(Cell::getFromRowCol(3, 5));
   ASSERT_ANY_THROW(SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, cells));

   cells = {Cell::getFromRowCol(0, 0), Cell::getFromRowCol(1, 1), Cell::getFromRowCol(1, 2)};
   retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, cells);
   ASSERT_FALSE(retval.has_value());

   cells = {Cell::getFromRowCol(0, 7), Cell::getFromRowCol(2, 6)};
   retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, cells);
   ASSERT_FALSE(retval.has_value());
}

TEST(SweepIntersectionElimination, PointingPair)
{
   CandidatesGrid grid;
   grid.setGiven(0, 2, 9);
   grid.setGiven(1, 1, 8);
   grid.setGiven(2, 2, 3);

   grid.setCandidates(0, 0, {2, 4, 5});
   grid.setCandidates(0, 1, {5, 6});
   grid.setCandidates(1, 0, {2, 5, 7});
   grid.setCandidates(1, 2, {1, 2, 5, 6, 7});
   grid.setCandidates(2, 0, {4, 5, 7});
   grid.setCandidates(2, 1, {5, 6, 7});

   Field expect = 670;
   ASSERT_EQ(grid.getNumCandidates(), expect);
   auto prv = grid;
   std::cout << grid << "\n";
   auto retval = SweepIntersectionElimination().sweep(grid);
   std::cout << CandidatesGrid::DifferenceToStrings(prv, grid) << "\n";
   ASSERT_TRUE(retval);
   expect -= 6;
   ASSERT_EQ(grid.getNumCandidates(), expect);
}
