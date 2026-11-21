#include <gtest/gtest.h>

#include "House.h"
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
