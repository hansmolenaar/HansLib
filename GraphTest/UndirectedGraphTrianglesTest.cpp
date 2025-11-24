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
}

TEST(UndirectedGraphTrianglesTest, Path2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const UndirectedGraphTriangles triangles(*graph);
    ASSERT_EQ(triangles.numTrianglesAt(0), 0);
    ASSERT_EQ(triangles.numTrianglesAt(1), 0);
    ASSERT_EQ(triangles.getSequence(), (std::vector<size_t>{0, 0}));
}

TEST(UndirectedGraphTrianglesTest, House)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::house);
    const UndirectedGraphTriangles triangles(*graph);
    ASSERT_EQ(triangles.numTrianglesAt(0), 0);
    ASSERT_EQ(triangles.numTrianglesAt(4), 1);
    ASSERT_EQ(triangles.getSequence(), (std::vector<size_t>{0, 0, 1, 1, 1}));
}
