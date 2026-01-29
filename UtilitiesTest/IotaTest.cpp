#include <gtest/gtest.h>

#include "Iota.h"

TEST(IotaTest, GetRange0)
{
    ASSERT_EQ(Iota::GetRange<int>(0).size(), 0);
    ASSERT_EQ(Iota::GetRange<int>(1).size(), 1);
    ASSERT_EQ(Iota::GetRange<int>(2).size(), 2);
}

TEST(IotaTest, GetRange2)
{
    std::vector<int> values;
    for (int i : Iota::GetRange<int>(2))
    {
        values.push_back(i);
    }
    const std::vector<int> expect{0, 1};
    ASSERT_EQ(values, expect);
}

TEST(IotaTest, GetRange1_3)
{
    std::vector<int> values;
    for (int i : Iota::GetRangeFromTo<int>(1, 3))
    {
        values.push_back(i);
    }
    const std::vector<int> expect{1, 2};
    ASSERT_EQ(values, expect);
}

TEST(IotaTest, GenerateIntVector)
{
    std::vector<int> values = Iota::GenerateIntVector(0);
    ASSERT_TRUE(values.empty());

    values = Iota::GenerateIntVector(2);
    std::vector<int> expect{0, 1};
    ASSERT_EQ(values, expect);

    values = Iota::GenerateIntVector(2, 3);
    expect = {3, 4};
    ASSERT_EQ(values, expect);
}

TEST(IotaTest, GenerateVector)
{
    std::vector<size_t> values = Iota::GenerateVector<size_t>(0);
    ASSERT_TRUE(values.empty());

    values = Iota::GenerateVector<size_t>(2);
    std::vector<size_t> expect{0, 1};
    ASSERT_EQ(values, expect);

    values = Iota::GenerateVector(2, size_t{3});
    expect = {3, 4};
    ASSERT_EQ(values, expect);
}

TEST(IotaTest, GetRangeFromTo)
{
    ASSERT_EQ(Iota::GetRangeFromTo<int>(0, 0).size(), 0);
    ASSERT_EQ(Iota::GetRangeFromTo<int>(1, 2).size(), 1);
    ASSERT_EQ(Iota::GetRangeFromTo<int>(1, 5).size(), 4);
    ASSERT_EQ(Iota::GetRangeFromTo<int>(3, 2).size(), 0);

    std::vector<int> values;
    for (int i : Iota::GetRangeFromTo(1, 3))
    {
        values.push_back(i);
    }
    ASSERT_EQ(values, (std::vector<int>{1, 2}));
}
