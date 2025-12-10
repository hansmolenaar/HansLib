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
        throw std::runtime_error("Implement me");
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
