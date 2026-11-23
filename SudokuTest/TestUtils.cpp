#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "House.h"
#include "IStrategy.h"
#include "StreamUtils.h"
#include "Sweep.h"
#include "TestModels.h"
#include "TestUtils.h"

using namespace Sudoku;

void TestUtils::CheckFieldValues(const CandidatesGrid &grid, const FieldValue::Container &fieldsValues)
{
   for (auto fv : fieldsValues)
   {
      ASSERT_TRUE(grid[fv.field].contains(fv.value));
   }
}

void TestUtils::CheckFieldInfo(const FieldsInfo &finfoOld)
{
   const FieldsInfo finfo(finfoOld.getGrid());
   ASSERT_EQ(finfoOld.getActiveFields(), finfo.getActiveFields());
   ASSERT_EQ(finfoOld.getSolvedFields(), finfo.getSolvedFields());
   for (House h : House::getAllHouses())
   {
      ASSERT_EQ(finfoOld.getHouseFields(h), finfo.getHouseFields(h));
   }
   for (Value v : ValueAll)
   {
      ASSERT_EQ(finfoOld.getFieldsWithValue(v), finfo.getFieldsWithValue(v));
      ASSERT_EQ(finfoOld.getActiveFieldsWithValue(v), finfo.getActiveFieldsWithValue(v));
   }
}

void TestUtils::CheckStrategyInterface(Sudoku::IStrategy &strategy)
{
   // full grid
   const CandidatesGrid grid;
   FieldsInfo finfo(grid);
   FieldValue::Container fieldValues;
   bool anyChange = strategy.collect(finfo, fieldValues);
   ASSERT_FALSE(anyChange);
   ASSERT_TRUE(fieldValues.empty());

   // solved
   const auto solved = TestModels::getSolvedGrid();
   finfo = FieldsInfo(solved);
   fieldValues = strategy.collect(finfo);
   ASSERT_TRUE(fieldValues.empty());
   ASSERT_FALSE(anyChange);
}

void TestUtils::CheckStrategy(Sudoku::IStrategy &strategy, const Sudoku::CandidatesGrid &grid,
                              const Sudoku::GridChange &expect)
{
   Sweep sweep(&strategy);
   for (auto eqv : CandidatesGrid::getSymmetricEquivalents(grid))
   {
      FieldsInfo finfo(eqv);
      FieldValue::Container fvs;
      const bool anyChange = strategy.collect(finfo, fvs);
      CheckFieldValues(eqv, fvs);
      ASSERT_EQ(fvs.empty(), !anyChange);
      ASSERT_EQ(fvs.size(), expect.numValuesChanged);
      auto numCandidates = finfo.getGrid().getNumCandidates();
      const auto changes = sweep.singleSweep(eqv, finfo);
      numCandidates -= finfo.getGrid().getNumCandidates();
      ASSERT_EQ(changes.numValuesChanged, numCandidates);
      ASSERT_EQ(changes.numValuesChanged, expect.numValuesChanged);
      ASSERT_EQ(changes.numFieldsSolved, expect.numFieldsSolved);
   }
}

void TestUtils::ShowStrategy(Sudoku::IStrategy &strategy, const Sudoku::CandidatesGrid &gridIn)
{
   CandidatesGrid grid = gridIn;
   FieldsInfo finfo(grid);
   Sweep sweep(&strategy);
   auto prv = grid;
   FieldValue::Container fvs;
   strategy.collect(finfo, fvs);
   auto changes = sweep.singleSweep(grid, finfo);
   std::cout << prv << "\n" << grid << "\n" << CandidatesGrid::DifferenceToStrings(prv, grid);
   EXPECT_EQ(changes.numValuesChanged, 0);
   EXPECT_EQ(changes.numFieldsSolved, 0);
   ASSERT_TRUE(false);
}
