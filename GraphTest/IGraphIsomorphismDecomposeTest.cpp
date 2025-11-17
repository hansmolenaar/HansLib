#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "GraphIsomorphismUtils.h"
#include "IGraphIsomorphismDecompose.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{

void CheckDecompose(const IGraphUs &graph, int expectNumLeaves = -1)
{
    const auto decomposed = IDecompose::Create(graph);
    const GraphIsomorphism::ToParentMap toParent(decomposed.get());

    int numLeaves = 0;
    std::vector<GraphVertex> vertices;
    for (const auto *leaf : toParent.getLeaves())
    {
        numLeaves += 1;
        const IGraphUs &graph = leaf->getSelf();
        const auto numVertices = graph.getNumVertices();
        for (GraphVertex v = 0; v < numVertices; ++v)
        {
            vertices.emplace_back(toParent.getVertexInRoot(v, leaf));
        }
    }

    if (expectNumLeaves > 0)
    {
        ASSERT_EQ(expectNumLeaves, numLeaves);
    }

    // Not vertices lost or added?
    const auto fullNumVertices = graph.getNumVertices();
    str::sort(vertices);
    ASSERT_EQ(vertices.size(), fullNumVertices);
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
    ASSERT_EQ(vertices.size(), fullNumVertices);
    ASSERT_EQ(vertices.front(), 0);
    ASSERT_EQ(vertices.back(), fullNumVertices - 1);
}

void CheckDecomposeList(const std::vector<std::string> &g6list)
{
    std::vector<std::unique_ptr<Graph::IGraphUs>> graphs = UndirectedGraphFromG6::getGraphs(g6list);

    for (const auto &graph : graphs)
    {
        CheckDecompose(*graph);
    }
}

} // namespace

TEST(IGraphIsomorphismDecomposeTest, GetToParentMapSingleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const auto decomposed = IDecompose::Create(*graph);
    ASSERT_EQ(decomposed->getTag(), (Tag{0}));
    const ToParentMap toParent(decomposed.get());
    ASSERT_EQ(toParent.size(), 1);
    ASSERT_TRUE(toParent.isRoot(decomposed.get()));

    CheckDecompose(*graph, 1);
}

TEST(IGraphIsomorphismDecomposeTest, GetToParentMapDisconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto decomposed = IDecompose::Create(*graph);
    ASSERT_EQ(decomposed->getTag(), (Tag{0}));
    const auto *root = decomposed.get();
    const ToParentMap toParent(root);
    ASSERT_EQ(toParent.size(), 1);
    ASSERT_TRUE(toParent.isRoot(decomposed.get()));
    ASSERT_EQ(toParent.getParent(root), nullptr);

    CheckDecompose(*graph, 1);
}

TEST(IGraphIsomorphismDecomposeTest, Disconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto decomposed = IDecompose::Create(*graph);
    ASSERT_EQ(decomposed->getTag(), (Tag{0}));
    CheckDecompose(*graph, 1);
}

TEST(IGraphIsomorphismDecomposeTest, EdgePlusVertex)
{
    const auto graph = UndirectedGraphFromG6::Create("BO");
    const auto decomposed = IDecompose::Create(*graph);
    ASSERT_EQ(decomposed->getTag(), (Tag{1, 1, 2}));
    const ToParentMap toParent(decomposed.get());

    const auto &tags = decomposed->getChildTags();
    ASSERT_EQ(tags.size(), 2);

    const auto *child0 =dynamic_cast<const DecomposeLeaf*>(Single( decomposed->getChildren(tags.at(0))));
    ASSERT_EQ(child0->getSelf().getNumVertices(), 2);
    ASSERT_EQ(child0->getTag(), (Tag{0}));
    ASSERT_TRUE(child0->isLeaf());

    const auto *child1 =dynamic_cast<const DecomposeLeaf*>(Single( decomposed->getChildren(tags.at(1))));
    ASSERT_EQ(child1->getSelf().getNumVertices(), 1);
    ASSERT_EQ(child1->getTag(), (Tag{0}));
    ASSERT_TRUE(child1->isLeaf());

    const auto tag0 = toParent.collectDecomposeTags(child0);
    const auto tag1 = toParent.collectDecomposeTags(child1);
    ASSERT_EQ(tag0, (std::vector<Tag>{Tag{0}, Tag{1, 1, 2}}));
    ASSERT_EQ(tag1, (std::vector<Tag>{Tag{0}, Tag{1, 1, 2}}));
    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, Paw)
{
    const auto graph = UndirectedGraphFromG6::Create("Cx");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, Dart)
{
    const auto graph = UndirectedGraphFromG6::Create("DvC");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, K5MinE)
{
    const auto graph = UndirectedGraphFromG6::Create("D~k");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, Gem)
{
    const auto graph = UndirectedGraphFromG6::Create("Dh{");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, X100)
{
    const auto graph = UndirectedGraphFromG6::Create("FgCNw");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, JustAskingQuestions)
{
    const std::vector<std::string> names{"F?S~", "FALcw"};
    const auto graph1 = UndirectedGraphFromG6::Create(names.at(0));
    const auto graph2 = UndirectedGraphFromG6::Create(names.at(1));
    TaggerNumbersFactory factory;
    const auto tagger1 = factory.createTagger(*graph1);
    const auto tagger2 = factory.createTagger(*graph2);
    const auto graphTag1 = tagger1->getGraphTagger()->getGraphTag();
    const auto graphTag2 = tagger2->getGraphTagger()->getGraphTag();
    const auto graphTagAll1 = IDecompose::GetGraphTags(*graph1);
    const auto graphTagAll2 = IDecompose::GetGraphTags(*graph2);
    ASSERT_NE(graphTagAll1, graphTagAll2);
    CheckDecomposeList(names);
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList3)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_3());
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList4)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_4());
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList5)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_5());
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList6)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_6());
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList7)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_7());
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList8)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_8());
}

TEST(IGraphIsomorphismDecomposeTest, SpecialCase1)
{
    const auto g0 = UndirectedGraphFromG6::CreateConnected("F@h^w");
    const auto g1 = UndirectedGraphFromG6::CreateConnected("F?lvw");

    const auto decompose0 = IDecompose::Create(*g0);
    const auto decompose1 = IDecompose::Create(*g1);
    const ToParentMap map0(decompose0.get());
    const ToParentMap map1(decompose1.get());
    const auto cmp = map0 <=> map1;
    ASSERT_TRUE(cmp != 0);
}
