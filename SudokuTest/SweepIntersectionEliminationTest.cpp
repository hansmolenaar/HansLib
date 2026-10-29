#include <gtest/gtest.h>

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
