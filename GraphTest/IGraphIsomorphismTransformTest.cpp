#include <gtest/gtest.h>

#include "Defines.h"
#include "IGraphIsomorphismTransform.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{

GraphVertex GetVertexInRoot(GraphVertex vertex, const IGraphIsomorphismTransform *transform,
                            const IGraphIsomorphismTransform::ToParentMap toParent)
{
    while (toParent.contains(transform))
    {
        vertex = transform->getVertexInParent(vertex);
        transform = toParent.at(transform);
    }
    return vertex;
}

void CheckTransform(const IGraphUs &graph, int expectNumLeaves = -1)
{
    const auto transformed = IGraphIsomorphismTransform::Create(graph);
    const auto toParent = IGraphIsomorphismTransform::GetToParentMap(transformed.get());

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
} // namespace

TEST(IGraphIsomorphismTransformTest, GetToParentMapSingleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const auto transformed = IGraphIsomorphismTransform::Create(*graph);
    const auto toParent = IGraphIsomorphismTransform::GetToParentMap(transformed.get());
    ASSERT_EQ(toParent.size(), 1);
    ASSERT_EQ(toParent.begin()->first, transformed.get());
    ASSERT_EQ(toParent.begin()->second, nullptr);

    CheckTransform(*graph, 1);
}

TEST(IGraphIsomorphismTransformTest, GetToParentMapDisconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto transformed = IGraphIsomorphismTransform::Create(*graph);
    const auto *root = transformed.get();
    const auto toParent = IGraphIsomorphismTransform::GetToParentMap(root);
    ASSERT_EQ(toParent.size(), 3);
    ASSERT_EQ(toParent.at(root), nullptr);

    CheckTransform(*graph, 2);
}

TEST(IGraphIsomorphismTransformTest, Disconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto transformed = IGraphIsomorphismTransform::Create(*graph);
    const auto &tag = Single(transformed->getChildTags());
    const auto &children = transformed->getChildren(tag);
    ASSERT_EQ(children.size(), 2);

    ASSERT_EQ(children.at(0)->getSelf().getNumVertices(), 1);
    ASSERT_EQ(children.at(0)->getVertexInParent(0), 0);

    ASSERT_EQ(children.at(1)->getSelf().getNumVertices(), 1);
    ASSERT_EQ(children.at(1)->getVertexInParent(0), 1);

    CheckTransform(*graph, 2);
}

TEST(IGraphIsomorphismTransformTest, EdgePlusVertex)
{
    const auto graph = UndirectedGraphFromG6::Create("BO");
    const auto transformed = IGraphIsomorphismTransform::Create(*graph);
    const auto &tags = transformed->getChildTags();
    ASSERT_EQ(tags.size(), 2);

    const auto &children0 = transformed->getChildren(tags.at(0));
    ASSERT_EQ(children0.size(), 1);
    ASSERT_EQ(children0.at(0)->getSelf().getNumVertices(), 2);

    const auto &children1 = transformed->getChildren(tags.at(1));
    ASSERT_EQ(children1.size(), 1);
    ASSERT_EQ(children1.at(0)->getSelf().getNumVertices(), 1);

    const auto &tagsGrandChildren0 = Single(children0)->getChildTags();
    ASSERT_TRUE(tagsGrandChildren0.empty());
    ASSERT_EQ(Single(children0)->getVertexInParent(0), 0);
    ASSERT_EQ(Single(children0)->getVertexInParent(1), 2);

    const auto &tagsGrandChildren1 = Single(children1)->getChildTags();
    ASSERT_TRUE(tagsGrandChildren1.empty());
    ASSERT_EQ(Single(children1)->getVertexInParent(0), 1);

    CheckTransform(*graph, 2);
}
