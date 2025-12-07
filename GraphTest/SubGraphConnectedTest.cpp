#include <gtest/gtest.h>

#include "Defines.h"
#include "Single.h"
#include "SubGraphConnected.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace Utilities;

TEST(SubGraphConnected, Basics)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const std::set<Vertex> indices{1, 2};
    SubGraphConnected subGraph(*graph, indices);

    ASSERT_EQ(subGraph.getNumVertices(), 2);
    ASSERT_EQ(subGraph.getNumEdges(), 1);

    std::vector<Vertex> ngbs;
    subGraph.setAdjacentVertices(1, ngbs);
    ASSERT_EQ(Single(ngbs), 0);

    ASSERT_TRUE(subGraph.areAdjacent(1, 0));
    ASSERT_EQ(subGraph.getDegree(1), 1);
    ASSERT_TRUE(str::equal(subGraph.getConnectedComponents(), std::vector<Vertex>{0, 0}));

    ASSERT_EQ(subGraph.getVertexInParent(0), 1);
    ASSERT_EQ(subGraph.getVertexInParent(1), 2);
}
