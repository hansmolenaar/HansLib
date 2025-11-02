#include <gtest/gtest.h>

#include "MyException.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UscGraph.h"

using namespace Graph;

TEST(UscGraphTest, ConnectedComponents)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const UscGraph uscGraph(*graph);

    ASSERT_EQ(uscGraph.getNumVertices(), 3);
    ASSERT_EQ(uscGraph.getNumEdges(), 2);
    ASSERT_EQ(uscGraph.getDegree(0), 1);
    ASSERT_EQ(uscGraph.getDegree(1), 2);
    ASSERT_EQ(uscGraph.getDegree(2), 1);
    ASSERT_FALSE(uscGraph.areAdjacent(0, 2));
    std::vector<GraphVertex> ngbs;
    uscGraph.setAdjacentVertices(0, ngbs);
    ASSERT_EQ(ngbs, std::vector<GraphVertex>{1});

    const auto components = uscGraph.getConnectedComponents();
    ASSERT_EQ(components, (std::vector<GraphVertex>{0, 0, 0}));
}

TEST(UscGraphTest, Disconnected)
{
    const auto graph = UndirectedGraphFromG6::Create("Eh?G");
    ASSERT_THROW(UscGraph tmp(*graph), MyException);
}
