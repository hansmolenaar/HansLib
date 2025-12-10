#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "IGraphIsomorphismTransform.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
void TestInterface(const GraphIsomorphism::ITransform &transform)
{
    ASSERT_FALSE(transform.getTagOfTransform().empty());
    ASSERT_FALSE(transform.getDescription().empty());
    const auto &children = transform.getChildren();
    if (children.empty())
    {
        const bool isKnown = dynamic_cast<const GraphIsomorphism::TransformKnown *>(&transform) != nullptr;
        const bool isFailure = dynamic_cast<const GraphIsomorphism::TransformFailure *>(&transform) != nullptr;
        ASSERT_TRUE(isKnown || isFailure);
    }
    else
    {
        Vertex numVertices = 0;
        for (const auto &kid : children)
        {
            const auto *subGraph = dynamic_cast<const Graph::SubGraph *>(&kid->getGraph());
            ASSERT_TRUE(subGraph != nullptr);
            numVertices += subGraph->getNumVertices();
        }
        ASSERT_EQ(transform.getGraph().getNumVertices(), numVertices);
    }
}
} // namespace

TEST(IGraphIsomorphismTransformTest, KnownPath4)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(4);
    const auto tgraph = std::make_shared<TaggedGraph>(*graph);
    const auto transform = TransformKnown::tryCreate(tgraph);
    ASSERT_NE(transform.get(), nullptr);
    TestInterface(*transform);
    ASSERT_EQ(transform->getTagOfTransform(), (Tag{1, 3, 4}));
    ASSERT_EQ(transform->getDescription(), "Known graph: path of order 4");
    ASSERT_TRUE(transform->getChildren().empty());
}

TEST(IGraphIsomorphismTransformTest, UnKnown)
{
    const auto graph = UndirectedGraphLibrary::Get_Butterfly();
    const auto tgraph = std::make_shared<TaggedGraph>(*graph);

    const auto transformKnown = TransformKnown::tryCreate(tgraph);
    ASSERT_TRUE(!transformKnown);

    const auto transform = ITransform::Create(tgraph);
    ASSERT_TRUE(!!transform);
    TestInterface(*transform);

    ASSERT_EQ(transform->getTagOfTransform(), (Tag{0}));
    ASSERT_EQ(transform->getDescription(), "Graph of order 5 cannot be transformed");
    ASSERT_TRUE(transform->getChildren().empty());
}

TEST(IGraphIsomorphismTransformTest, DisconnectedComplementPath2)
{
    const auto graphComplement = UndirectedGraphLibrary::Get_Path(2);
    const auto graph = UndirectedGraph::CreateComplement(*graphComplement);
    const auto tgraph = std::make_shared<TaggedGraph>(graph);
    const auto transform = TransformDisconnected::tryCreate(tgraph);
    ASSERT_NE(transform.get(), nullptr);
    TestInterface(*transform);
    ASSERT_EQ(transform->getTagOfTransform(), (Tag{2, 1, 1}));
    ASSERT_EQ(transform->getDescription(), "Disconnected graph with components of order: 1 1");
    ASSERT_EQ(transform->getChildren().size(), 2);
}

TEST(IGraphIsomorphismTransformTest, DisconnectedComplementDiamond)
{
    const auto graphComplement = UndirectedGraphLibrary::Get_Diamond();
    const auto graph = UndirectedGraph::CreateComplement(*graphComplement);
    const auto tgraph = std::make_shared<TaggedGraph>(graph);
    const auto transform = TransformDisconnected::tryCreate(tgraph);
    ASSERT_NE(transform.get(), nullptr);
    TestInterface(*transform);
    ASSERT_EQ(transform->getTagOfTransform(), (Tag{2, 1, 1, 2}));
    ASSERT_EQ(transform->getDescription(), "Disconnected graph with components of order: 1 1 2");
    ASSERT_EQ(transform->getChildren().size(), 3);
}

TEST(IGraphIsomorphismTransformTest, DisconnectedPath2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const auto tgraph = std::make_shared<TaggedGraph>(*graph);
    const auto transform = TransformDisconnected::tryCreate(tgraph);
    ASSERT_EQ(transform.get(), nullptr);
}
