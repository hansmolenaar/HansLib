#include <gtest/gtest.h>

#include "Combinations.h"
#include "Defines.h"

#include <set>

TEST(CombinationsTest, Test53)
{
    const auto allCombinations = Combinations::Get(5, 3);
    ASSERT_EQ(allCombinations.size(), 10);

    ASSERT_TRUE(str::all_of(allCombinations, [](const auto &cmb) { return cmb.size() == 5; }));

    std::set<std::vector<bool>> unique;
    for (const auto &bv : allCombinations)
    {
        unique.insert(bv);
    }
    ASSERT_EQ(unique.size(), 10);
}
