#include <gtest/gtest.h>

#include "GraphIsomorphismTaggerTriangles.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;

TEST(GraphIsomorphismTaggerMaxDegreeTest, CheckTriangulation)
{
    std::vector<std::array<GraphVertex, 3>> allTriangles;
    std::vector<std::array<GraphVertex, 3>> expect;

    auto graph = UndirectedGraphLibrary::Get_Path(3);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {};
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphLibrary::Get_Cycle(3);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {std::array<GraphVertex, 3>{0, 1, 2}};
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::pan3);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {std::array<GraphVertex, 3>{0, 1, 2}};
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::fish);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {std::array<GraphVertex, 3>{3, 4, 5}};
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::house);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {std::array<GraphVertex, 3>{2, 3, 4}};
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::butterfly);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {
        std::array<GraphVertex, 3>{0, 1, 2},
        std::array<GraphVertex, 3>{0, 3, 4},
    };
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::diamond);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {
        std::array<GraphVertex, 3>{0, 1, 2},
        std::array<GraphVertex, 3>{1, 2, 3},
    };
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphLibrary::Get_CompleteGraph(4);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {
        std::array<GraphVertex, 3>{0, 1, 2},
        std::array<GraphVertex, 3>{0, 1, 3},
        std::array<GraphVertex, 3>{0, 2, 3},
        std::array<GraphVertex, 3>{1, 2, 3},
    };
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphLibrary::Get_CompleteGraph(5);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {
        std::array<GraphVertex, 3>{0, 1, 2},
        std::array<GraphVertex, 3>{0, 1, 3},
        std::array<GraphVertex, 3>{0, 1, 4},
        std::array<GraphVertex, 3>{0, 2, 3},
        std::array<GraphVertex, 3>{0, 2, 4},
        std::array<GraphVertex, 3>{0, 3, 4},
        std::array<GraphVertex, 3>{1, 2, 3},
        std::array<GraphVertex, 3>{1, 2, 4},
        std::array<GraphVertex, 3>{1, 3, 4},
        std::array<GraphVertex, 3>{2, 3, 4},
    };
    ASSERT_EQ(allTriangles, expect);

}
