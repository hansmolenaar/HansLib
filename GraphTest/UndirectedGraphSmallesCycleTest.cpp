#include <gtest/gtest.h>

#include "Iota.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UndirectedGraphSmallestCycle.h"

using namespace Graph;

TEST(UndirectedGraphSmallestCycleTest, Disconnected)
{
    for (Vertex n : Iota::GetRange(5))
    {
        const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(n);
        const UndirectedGraphSmallestCycle smallesCycles(*graph);
        const std::vector<Vertex> expect(n, UndirectedGraphSmallestCycle::noCycleLength);
        ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), expect);
    }
}

TEST(UndirectedGraphSmallestCycleTest, Cycle)
{
    for (Vertex n : Iota::GetRangeFromTo(3, 10))
    {
        const auto graph = UndirectedGraphLibrary::Get_Cycle(n);
        const UndirectedGraphSmallestCycle smallesCycles(*graph);
        const std::vector<Vertex> expect(n, n);
        ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), expect);
    }
}

TEST(UndirectedGraphSmallestCycleTest, Path)
{
    for (Vertex n : Iota::GetRangeFromTo(2, 10))
    {
        const auto graph = UndirectedGraphLibrary::Get_Path(n);
        const UndirectedGraphSmallestCycle smallesCycles(*graph);
        const std::vector<Vertex> expect(n, UndirectedGraphSmallestCycle::noCycleLength);
        ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), expect);
    }
}

TEST(UndirectedGraphSmallestCycleTest, Diamond)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::diamond);
    const UndirectedGraphSmallestCycle smallesCycles(*graph);
    ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), (std::vector<Vertex>{3, 3, 3, 3}));
}

TEST(UndirectedGraphSmallestCycleTest, X84)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::X84);
    const UndirectedGraphSmallestCycle smallesCycles(*graph);
    ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), (std::vector<Vertex>{4, 4, 4, 4, 0, 0}));
}

TEST(UndirectedGraphSmallestCycleTest, Pan3)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const UndirectedGraphSmallestCycle smallesCycles(*graph);
    ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), (std::vector<Vertex>{3, 3, 3, 0}));
}

TEST(UndirectedGraphSmallestCycleTest, Claw)
{
    const auto graph = UndirectedGraphLibrary::Get_Claw();
    const UndirectedGraphSmallestCycle smallesCycles(*graph);
    ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), (std::vector<Vertex>{0, 0, 0, 0}));
}
