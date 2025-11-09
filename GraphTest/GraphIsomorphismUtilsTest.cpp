#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismUtils.h"

using namespace Graph;
using namespace GraphIsomorphism;

TEST(GraphIsomorphismUtilsTest, CondenseSizeSequence_int)
{
    const std::vector<int> sizes{2, 5, 2};
    const auto tag = GraphIsomorphismUtils::CondenseSizeSequence(sizes);
    ASSERT_TRUE(str::equal(tag, (Tag{2, 2, 5, 1})));
}

TEST(GraphIsomorphismUtilsTest, CondenseSizeSequence_size_t)
{
    const std::vector<size_t> sizes{2, 5, 2};
    const auto tag = GraphIsomorphismUtils::CondenseSizeSequence(sizes);
    ASSERT_TRUE(str::equal(tag, (Tag{2, 2, 5, 1})));
}
