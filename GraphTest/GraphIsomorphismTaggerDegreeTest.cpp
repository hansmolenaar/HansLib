#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerDegreeTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    TaggerDegreeFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory);

    const TaggerDegree tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(), Tag{1, 1, 2}));
}

TEST(GraphIsomorphismTaggerDegreeTest, Star111)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 2, 1});
    TaggerDegreeFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory);

    const auto tagger = TaggerDegree(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(), Tag{1, 1, 1, 2, 3}));
}
