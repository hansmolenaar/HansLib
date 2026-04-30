#include <gtest/gtest.h>

#include "GraphIsomorphismStatus.h"
#include "MyAssert.h"

using namespace GraphIsomorphism;
using namespace Graph;
using namespace Utilities;

namespace
{
} // namespace

TEST(GraphIsomorphismStatusTest, DefaultConstructor)
{
    const Status status;
    ASSERT_EQ(status.getFlag(), Flag::NotIsomorphic);
    ASSERT_TRUE(status.getVertexPairs().empty());
}

TEST(GraphIsomorphismStatusTest, Constructor)
{
    GraphIsomorphism::Status status(3);
    ASSERT_EQ(status.getFlag(), Flag::Undecided);
    ASSERT_TRUE(status.getVertexPairs().empty());

    status.addPair(VertexPair{1, 2});
    ASSERT_TRUE(status.containsFirst(1));
    ASSERT_FALSE(status.containsFirst(2));
    ASSERT_TRUE(status.containsSecond(2));
    ASSERT_FALSE(status.containsSecond(1));
}

TEST(GraphIsomorphismStatusTest, AddCheck)
{
    GraphIsomorphism::Status status(3);

    status.addPair(VertexPair{1, 2});
    ASSERT_THROW(status.addPair(VertexPair{1, 3}), MyException);
    ASSERT_THROW(status.addPair(VertexPair{0, 2}), MyException);
}
