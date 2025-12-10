#include <gtest/gtest.h>

#include "GraphIsomorphismDecomposeNode.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;

TEST(GraphIsomorphismDecomposeNodeTest, Cycle5)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(5);
    const auto tgraph = std::make_shared<TaggedGraph>(*graph);
    const auto decomposeNode = DecomposeNode::Create(tgraph);
    ASSERT_EQ(graph->getName(), decomposeNode->getGraph().getName());
    ASSERT_EQ(decomposeNode->getTag(), (Tag{1, 2, 5}));
}
