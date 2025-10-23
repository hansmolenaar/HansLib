#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouper.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismGrouperTest, DegreePath3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);

    const auto taggerDegree = TaggerDegree(*graph);
    const auto groupingDegree = Grouper{}(taggerDegree);

    ASSERT_TRUE(str::equal(groupingDegree, GraphGroup{0, 1, 0}));
}

TEST(GraphIsomorphismGrouperTest, Star112)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 1, 2});

    const auto taggerDegree = TaggerDegree(*graph);
    const auto groupingDegree = Grouper{}(taggerDegree);

    ASSERT_TRUE(str::equal(groupingDegree, GraphGroup{0, 1, 1, 2, 1}));
}
