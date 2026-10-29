#include <gtest/gtest.h>

#include "SweepIntersectionElimination.h"

using namespace Sudoku;

TEST(SweepIntersectionElimination, BoxRow)
{
   std::vector<Field> fields{0, 2};
   std::optional<HouseId> retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields);
   ASSERT_TRUE(retval.has_value());
   ASSERT_EQ(retval.value(), (HouseId{HouseType::Row, 0}));

   fields.push_back(1);
   retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields);
   ASSERT_TRUE(retval.has_value());
   ASSERT_EQ(retval.value(), (HouseId{HouseType::Row, 0}));

   fields.push_back(3);
   ASSERT_ANY_THROW(SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields));

   fields = {0, 1, 11};
   retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields);
   ASSERT_FALSE(retval.has_value());

   fields = {0, 10};
   retval = SweepIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields);
   ASSERT_FALSE(retval.has_value());
}
