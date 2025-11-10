#include <gtest/gtest.h>

#include "Defines.h"
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

GraphVertex GetVertexInRoot(GraphVertex vertex, const IGraphIsomorphismDecompose *decompose,
                            const IGraphIsomorphismDecompose::ToParentMap toParent)
{
    while (toParent.contains(decompose))
    {
        vertex = decompose->getVertexInParent(vertex);
        decompose = toParent.at(decompose);
    }
    return vertex;
}

void CheckDecompose(const IGraphUs &graph, int expectNumLeaves = -1)
{
    const auto decomposeed = IGraphIsomorphismDecompose::Create(graph);
    const auto toParent = IGraphIsomorphismDecompose::GetToParentMap(decomposeed.get());

    int numLeaves = 0;
    std::vector<GraphVertex> vertices;
    for (const auto &itr : toParent)
    {
        if (itr.first->isLeaf())
        {
            numLeaves += 1;
            const IGraphUs &graph = itr.first->getSelf();
            const auto numVertices = graph.getNumVertices();
            for (GraphVertex v = 0; v < numVertices; ++v)
            {
                vertices.emplace_back(GetVertexInRoot(v, itr.first, toParent));
            }
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

void CheckDecomposeList(const std::vector<std::string> &g6list, Tag expectMultiplicities)
{
    const std::vector<std::unique_ptr<Graph::IGraphUs>> graphs = UndirectedGraphFromG6::getGraphs(g6list);
    std::map<GraphTags, size_t> multiplicityMap;
    for (const auto &g : graphs)
    {
        CheckDecompose(*g);
        multiplicityMap[IGraphIsomorphismDecompose::GetGraphTags(*g)] += 1;
    }

    std::vector<size_t> multiplicities;
    for (const auto &itr : multiplicityMap)
    {
        multiplicities.push_back(itr.second);
    }
    const auto tag = CondenseSizeSequence(multiplicities);

    if (!str::equal(tag, expectMultiplicities))
    {
        ASSERT_TRUE(false);
    }
}

} // namespace

TEST(IGraphIsomorphismDecomposeTest, GetToParentMapSingleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const auto decomposeed = IGraphIsomorphismDecompose::Create(*graph);
    const auto toParent = IGraphIsomorphismDecompose::GetToParentMap(decomposeed.get());
    ASSERT_EQ(toParent.size(), 1);
    ASSERT_EQ(toParent.begin()->first, decomposeed.get());
    ASSERT_EQ(toParent.begin()->second, nullptr);

    CheckDecompose(*graph, 1);
}

TEST(IGraphIsomorphismDecomposeTest, GetToParentMapDisconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto decomposeed = IGraphIsomorphismDecompose::Create(*graph);
    const auto *root = decomposeed.get();
    const auto toParent = IGraphIsomorphismDecompose::GetToParentMap(root);
    ASSERT_EQ(toParent.size(), 3);
    ASSERT_EQ(toParent.at(root), nullptr);

    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, Disconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto decomposeed = IGraphIsomorphismDecompose::Create(*graph);
    const auto &tag = Single(decomposeed->getChildTags());
    const auto &children = decomposeed->getChildren(tag);
    ASSERT_EQ(children.size(), 2);

    ASSERT_EQ(children.at(0)->getSelf().getNumVertices(), 1);
    ASSERT_EQ(children.at(0)->getVertexInParent(0), 0);

    ASSERT_EQ(children.at(1)->getSelf().getNumVertices(), 1);
    ASSERT_EQ(children.at(1)->getVertexInParent(0), 1);

    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, EdgePlusVertex)
{
    const auto graph = UndirectedGraphFromG6::Create("BO");
    const auto decomposeed = IGraphIsomorphismDecompose::Create(*graph);
    const auto &tags = decomposeed->getChildTags();
    ASSERT_EQ(tags.size(), 2);

    const auto &children0 = decomposeed->getChildren(tags.at(0));
    ASSERT_EQ(children0.size(), 1);
    ASSERT_EQ(children0.at(0)->getSelf().getNumVertices(), 2);

    const auto &children1 = decomposeed->getChildren(tags.at(1));
    ASSERT_EQ(children1.size(), 1);
    ASSERT_EQ(children1.at(0)->getSelf().getNumVertices(), 1);

    const auto &tagsGrandChildren0 = Single(children0)->getChildTags();
    ASSERT_TRUE(tagsGrandChildren0.empty());
    ASSERT_EQ(Single(children0)->getVertexInParent(0), 0);
    ASSERT_EQ(Single(children0)->getVertexInParent(1), 2);

    const auto &tagsGrandChildren1 = Single(children1)->getChildTags();
    ASSERT_TRUE(tagsGrandChildren1.empty());
    ASSERT_EQ(Single(children1)->getVertexInParent(0), 1);

    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    const auto decomposeed = IGraphIsomorphismDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, Paw)
{
    const auto graph = UndirectedGraphFromG6::Create("Cx");
    const auto decomposeed = IGraphIsomorphismDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, Dart)
{
    const auto graph = UndirectedGraphFromG6::Create("DvC");
    const auto decomposeed = IGraphIsomorphismDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, K5MinE)
{
    const auto graph = UndirectedGraphFromG6::Create("D~k");
    const auto decomposeed = IGraphIsomorphismDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, Gem)
{
    const auto graph = UndirectedGraphFromG6::Create("Dh{");
    const auto decomposeed = IGraphIsomorphismDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, X100)
{
    const auto graph = UndirectedGraphFromG6::Create("FgCNw");
    const auto decomposeed = IGraphIsomorphismDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList3)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_3(), Tag{1, 4});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList4)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_4(), Tag{1, 11});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList5)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_5(), {1, 30, 2, 2});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList6)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_6(), {1, 74, 2, 16, 3, 5, 4, 6, 5, 2});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList7)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_7(),
                       {1, 105, 2, 39, 3, 12, 4, 4, 5, 3, 6, 6, 7, 1, 9, 1});
}
