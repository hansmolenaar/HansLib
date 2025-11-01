#include <gtest/gtest.h>

#include "GraphIsomorphismTagFlyweight.h"

using namespace GraphIsomorphism;

TEST(GraphIsomorphismTagFlyweightTest, Test)
{
    const Tag &tag0 = TagFlyweight::getSingleEntryTag(0);
    for (TagEntry entry = 0; entry < 100; ++entry)
    {
        const auto &tag = TagFlyweight::getSingleEntryTag(entry);
        ASSERT_EQ(tag.size(), 1);
        ASSERT_EQ(tag.front(), entry);
    }
    const Tag &tag0_again = TagFlyweight::getSingleEntryTag(0);
    ASSERT_EQ(&tag0, &tag0_again);
}
