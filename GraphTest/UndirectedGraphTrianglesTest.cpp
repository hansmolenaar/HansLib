#include <gtest/gtest.h>

#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UndirectedGraphTriangles.h"

using namespace Graph;

TEST(UndirectedGraphTrianglesTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const UndirectedGraphTriangles triangles(*graph);
    ASSERT_EQ(triangles.getSequence(), (std::vector<size_t>{1, 1, 1}));
}

TEST(UndirectedGraphTrianglesTest, Pan3)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const UndirectedGraphTriangles triangles(*graph);
    ASSERT_EQ(triangles.numTrianglesAt(0), 1);
    ASSERT_EQ(triangles.numTrianglesAt(1), 1);
    ASSERT_EQ(triangles.numTrianglesAt(2), 1);
    ASSERT_EQ(triangles.numTrianglesAt(3), 0);
    ASSERT_EQ(triangles.getSequence(), (std::vector<size_t>{1, 1, 1, 0}));

    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(0), 2);
    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(3), 0);
}

TEST(UndirectedGraphTrianglesTest, Path2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const UndirectedGraphTriangles triangles(*graph);
    ASSERT_EQ(triangles.numTrianglesAt(0), 0);
    ASSERT_EQ(triangles.numTrianglesAt(1), 0);
    ASSERT_EQ(triangles.getSequence(), (std::vector<size_t>{0, 0}));

    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(0), 0);
    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(1), 0);
}

TEST(UndirectedGraphTrianglesTest, House)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::house);
    const UndirectedGraphTriangles triangles(*graph);
    ASSERT_EQ(triangles.numTrianglesAt(0), 0);
    ASSERT_EQ(triangles.numTrianglesAt(4), 1);
    ASSERT_EQ(triangles.getSequence(), (std::vector<size_t>{0, 0, 1, 1, 1}));

    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(0), 0);
    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(1), 0);
    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(2), 2);
    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(3), 2);
    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(4), 2);
}

TEST(UndirectedGraphTrianglesTest, Butterfly)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::butterfly);
    const UndirectedGraphTriangles triangles(*graph);
    ASSERT_EQ(triangles.numTrianglesAt(0), 2);
    ASSERT_EQ(triangles.numTrianglesAt(4), 1);

    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(0), 4);
    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(4), 2);
}

TEST(UndirectedGraphTrianglesTest, Diamond)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::diamond);
    const UndirectedGraphTriangles triangles(*graph);
    ASSERT_EQ(triangles.numTrianglesAt(0), 1);
    ASSERT_EQ(triangles.numTrianglesAt(1), 2);

    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(0), 2);
    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(1), 3);
    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(2), 3);
    ASSERT_EQ(triangles.numNeighborsInTrianglesAt(3), 2);
}
