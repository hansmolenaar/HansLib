#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerMaxDegree.h"
#include "GraphUsc.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerMaxDegreeTest, CheckVertexTagger)
{
    TaggerMaxDegreeFactory factory;
    GraphTest::CheckVertexTagger(factory);
}

TEST(GraphIsomorphismTaggerMaxDegreeTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    TaggerMaxDegreeFactory factory;
    GraphTest::CheckVertexTaggerConsistency(*graph, factory, 1);

    const TaggerMaxDegree tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{0}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{0}));

    ASSERT_EQ(tagger.getNumVertices(), 3);
}

TEST(GraphIsomorphismTaggerMaxDegreeTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    TaggerMaxDegreeFactory factory;
    GraphTest::CheckVertexTaggerConsistency(*graph, factory, 2);

    const auto vtagger = factory.createTagger(*graph);
    const auto *tagger = vtagger->getVertexTagger();
    ASSERT_TRUE(str::equal(tagger->getVertexTag(0), Tag{0}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(1), Tag{1}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(2), Tag{2}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(3), Tag{0}));
}
