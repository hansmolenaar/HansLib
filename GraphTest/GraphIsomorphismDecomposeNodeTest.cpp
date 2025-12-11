#include <gtest/gtest.h>

#include "GraphIsomorphismDecomposeNode.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;

namespace
{
void TestInterface(const DecomposeNode &decomposeNode)
{
    const auto tag = decomposeNode.getTag();
    ASSERT_FALSE(tag.empty());
    const auto descr = decomposeNode.getDescription();
    ASSERT_FALSE(descr.empty());
    if (decomposeNode.isLeaf())
    {
        ASSERT_EQ(decomposeNode.getGroupingChildren().getGroupSizes().size(), 0);
    }
    else
    {
        std::vector<Vertex> verticesInParent;
        const auto &grouping = decomposeNode.getGroupingChildren()();
        for (const auto &group : grouping)
        {
            for (const auto *part : group)
            {
                for (Vertex vertex = 0; vertex < part->getGraph().getNumVertices(); ++vertex)
                {
                    verticesInParent.push_back(part->getVertexInParent(vertex));
                }
            }
        }
        const auto nVertices = decomposeNode.getGraph().getNumVertices();
        ASSERT_EQ(verticesInParent.size(), nVertices);
        str::sort(verticesInParent);
        verticesInParent.erase(std::unique(verticesInParent.begin(), verticesInParent.end()), verticesInParent.end());
        ASSERT_EQ(verticesInParent.back() + 1, nVertices);
        ASSERT_EQ(verticesInParent.size(), nVertices);
    }

    ASSERT_EQ(decomposeNode <=> decomposeNode, std::weak_ordering::equivalent);
}

} // namespace

TEST(GraphIsomorphismDecomposeNodeTest, Cycle5)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(5);
    const auto tgraph = std::make_shared<TaggedGraph>(*graph);
    const auto decomposeNode = DecomposeNode::Create(tgraph);
    TestInterface(*decomposeNode);
    ASSERT_EQ(graph->getName(), decomposeNode->getGraph().getName());
    ASSERT_EQ(decomposeNode->getTag(), (Tag{1, 2, 5}));
}

TEST(GraphIsomorphismDecomposeNodeTest, NotTransformable)
{
    // May change
    const auto graph = UndirectedGraphLibrary::Get_Star({3, 5, 7});
    const auto tgraph = std::make_shared<TaggedGraph>(*graph);
    const auto decomposeNode = DecomposeNode::Create(tgraph);
    TestInterface(*decomposeNode);
    ASSERT_EQ(graph->getName(), decomposeNode->getGraph().getName());
    ASSERT_EQ(decomposeNode->getTag(), (Tag{0}));
}

TEST(GraphIsomorphismDecomposeNodeTest, TwoEdges)
{
    const auto graphComplement = UndirectedGraphLibrary::Get_Cycle(4);
    const auto graph = UndirectedGraph::CreateComplement(*graphComplement);
    const auto tgraph = std::make_shared<TaggedGraph>(graph);
    const auto decomposeNode = DecomposeNode::Create(tgraph);
    const auto descr = decomposeNode->getDescription();
    ASSERT_EQ(descr, "Disconnected graph with components of order: 2 2");
    TestInterface(*decomposeNode);
    ASSERT_EQ(decomposeNode->getTag(), (Tag{2, 2, 2}));

    const auto &groupingChildren = decomposeNode->getGroupingChildren();
    ASSERT_EQ(groupingChildren.getGroupSizes(), (std::vector<size_t>{2}));
}

TEST(GraphIsomorphismDecomposeNodeTest, CoDiamond)
{
    const auto graphComplement = UndirectedGraphLibrary::Get_Diamond();
    const auto graph = UndirectedGraph::CreateComplement(*graphComplement);
    const auto tgraph = std::make_shared<TaggedGraph>(graph);
    const auto decomposeNode = DecomposeNode::Create(tgraph);
    const auto descr = decomposeNode->getDescription();
    ASSERT_EQ(descr, "Disconnected graph with components of order: 1 1 2");
    const auto &groupingChildren = decomposeNode->getGroupingChildren();
    ASSERT_EQ(groupingChildren.getGroupSizes(), (std::vector<size_t>{2, 1}));
    TestInterface(*decomposeNode);
    ASSERT_EQ(decomposeNode->getTag(), (Tag{2, 1, 1, 2}));
}
