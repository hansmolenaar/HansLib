#include <gtest/gtest.h>

#include "Defines.h"
#include "ValueSetUtils.h"

using namespace Sudoku;

TEST(ValueSetUtils, IsSolvedSucces)
{
    ValueSet values;
    for (auto n : RowColBoxPositionsAll)
    {
        values.at(n) = 9 - n;
    }
    ASSERT_TRUE(ValueSetUtils::IsSolved(values));
}

TEST(ValueSetUtils, IsSolvedNotSolved)
{
    ValueSet values;
    str::fill(values, ValueUndefined);
    ASSERT_FALSE(ValueSetUtils::IsSolved(values));
}

TEST(ValueSetUtils, IsSolvedDuplicate)
{
    ValueSet values;
    str::fill(values, ValueUndefined);
    values.at(2) = 5;
    values.at(5) = 5;
    ASSERT_THROW(ValueSetUtils::IsSolved(values), MyException);
}

TEST(ValueSetUtils, IsSolvedOutOfBounds)
{
    ValueSet values;
    str::fill(values, ValueUndefined);
    values.at(2) = 10;
    ASSERT_THROW(ValueSetUtils::IsSolved(values), MyException);
}
