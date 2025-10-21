#include <gtest/gtest.h>

#include "StreamUtils.h"
#include <sstream>
#include <vector>

TEST(StreamUtilsTest, InsertListBase)
{
    std::ostringstream os;
    const std::vector<int> list{1, 2};
    StreamUtils::insertList(os, list);
    ASSERT_EQ(os.str(), "(1, 2)");
}

TEST(StreamUtilsTest, InsertListEmpty)
{
    std::ostringstream os;
    const std::vector<int> list;
    StreamUtils::insertList(os, list, "[", ",", "]");
    ASSERT_EQ(os.str(), "[]");
}