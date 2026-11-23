#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "StrategyIntersectionElimination.h"
#include "TestUtils.h"

using namespace Sudoku;

TEST(StrategyIntersectionElimination, Strategy)
{
   StrategyIntersectionElimination strategy;
   TestUtils::CheckStrategyInterface(strategy);
}

TEST(StrategyIntersectionElimination, BoxRow)
{
   const CandidatesGrid grid;
   const FieldsInfo finfo(grid);
   FieldBits fields;
   fields.set(0);
   fields.set(2);
   std::optional<House> retval =
       StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields, finfo);
   ASSERT_TRUE(retval.has_value());
   ASSERT_EQ(retval.value().houseType, HouseType::Row);
   ASSERT_EQ(retval.value().houseIndex, 0);

   fields.set(1);
   retval = StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields, finfo);
   ASSERT_TRUE(retval.has_value());
   ASSERT_EQ(retval.value().houseType, HouseType::Row);
   ASSERT_EQ(retval.value().houseIndex, 0);

   fields.set(3);
   ASSERT_ANY_THROW(StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields, finfo));

   fields.reset();
   fields.set(0);
   fields.set(1);
   fields.set(11);
   retval = StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields, finfo);
   ASSERT_FALSE(retval.has_value());

   fields.reset();
   fields.set(0);
   fields.set(10);
   retval = StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields, finfo);
   ASSERT_FALSE(retval.has_value());
}

TEST(StrategyIntersectionElimination, BoxCol)
{
   const CandidatesGrid grid;
   const FieldsInfo finfo(grid);
   FieldBits fields;
   fields.set(Cell::getFromRowCol(0, 5).field);
   fields.set(Cell::getFromRowCol(2, 5).field);

   std::optional<House> retval =
       StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields, finfo);
   ASSERT_TRUE(retval.has_value());
   ASSERT_EQ(retval.value().houseType, HouseType::Col);
   ASSERT_EQ(retval.value().houseIndex, 5);

   fields.set(Cell::getFromRowCol(1, 5).field);
   retval = StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields, finfo);
   ASSERT_TRUE(retval.has_value());
   ASSERT_EQ(retval.value(), (House{HouseType::Col, 5}));

   fields.set(Cell::getFromRowCol(3, 5).field);
   ASSERT_ANY_THROW(StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields, finfo));

   fields.reset();
   fields.set(Cell::getFromRowCol(0, 0).field);
   fields.set(Cell::getFromRowCol(1, 1).field);
   fields.set(Cell::getFromRowCol(1, 2).field);
   retval = StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields, finfo);
   ASSERT_FALSE(retval.has_value());

   fields.reset();
   fields.set(Cell::getFromRowCol(0, 7).field);
   fields.set(Cell::getFromRowCol(2, 6).field);
   retval = StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType::Box, fields, finfo);
   ASSERT_FALSE(retval.has_value());
}

TEST(StrategyIntersectionElimination, PointingPairCol)
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

   StrategyIntersectionElimination strategy;
   FieldsInfo finfo(grid);

   FieldValue::Container fieldValues;
   const bool anyChange = strategy.collect(finfo, fieldValues);
   ASSERT_TRUE(anyChange);
   ASSERT_EQ(fieldValues.size(), 6);

   TestUtils::CheckStrategy(strategy, grid, {6, 0});
}

TEST(StrategyIntersectionElimination, PointingPairRow)
{
   CandidatesGrid grid;
   grid.setGiven(6, 3, 5);
   grid.setGiven(6, 4, 2);
   grid.setGiven(6, 5, 8);
   grid.setGiven(7, 3, 6);
   grid.setGiven(7, 4, 1);

   grid.setCandidates(7, 5, {4, 7});
   grid.setCandidates(8, 3, {3, 4, 7, 9});
   grid.setCandidates(8, 4, {3, 7, 9});
   grid.setCandidates(8, 5, {3, 4, 7});

   StrategyIntersectionElimination strategy;
   FieldsInfo finfo(grid);
   TestUtils::CheckStrategy(strategy, grid, {12, 0});

   FieldValue::Container fieldValues;
   StrategyIntersectionElimination().collect(finfo, fieldValues);
   ASSERT_EQ(fieldValues.size(), 12);

   Field expect = 665;
   ASSERT_EQ(grid.getNumCandidates(), expect);
   const auto retval = grid.apply(fieldValues, finfo);
   ASSERT_EQ(retval.numValuesChanged, 12);
   ASSERT_EQ(retval.numFieldsSolved, 0);

   expect -= 12;
   ASSERT_EQ(grid.getNumCandidates(), expect);
}

TEST(StrategyIntersectionElimination, BoxLine)
{
   CandidatesGrid grid;
   grid.setGiven(0, 5, 7);
   grid.setGiven(0, 1, 1);
   grid.setGiven(0, 2, 6);
   grid.setGiven(0, 6, 8);
   grid.setGiven(0, 8, 3);
   grid.setGiven(1, 3, 8);
   grid.setGiven(2, 5, 1);

   grid.setCandidates(0, 0, {4, 5});
   grid.setCandidates(0, 3, {2, 4, 5});
   grid.setCandidates(0, 4, {2, 4, 5, 9});
   grid.setCandidates(0, 7, {4, 9});
   grid.setCandidates(1, 4, {2, 3, 4, 5, 6});
   grid.setCandidates(1, 5, {3, 4, 5, 6});
   grid.setCandidates(2, 3, {2, 3, 4, 5});
   grid.setCandidates(2, 4, {2, 3, 4, 5, 9});

   StrategyIntersectionElimination strategy;
   FieldsInfo finfo(grid);
   TestUtils::CheckStrategy(strategy, grid, {15, 0});

   FieldValue::Container fieldValues;
   StrategyIntersectionElimination().collect(finfo, fieldValues);
   ASSERT_EQ(fieldValues.size(), 15);

   Field expect = 630;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   const auto retval = grid.applyAndUpdate(fieldValues, finfo);
   ASSERT_TRUE(retval);
   expect -= 15;
   ASSERT_EQ(grid.getNumCandidates(), expect);
   ASSERT_FALSE(grid[Cell::getFromRowCol(1, 4).field].contains(2));
   ASSERT_FALSE(grid[Cell::getFromRowCol(2, 3).field].contains(2));
   ASSERT_FALSE(grid[Cell::getFromRowCol(2, 4).field].contains(2));
}
