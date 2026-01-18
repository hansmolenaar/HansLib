#include <gtest/gtest.h>

#include "UndirectedGraphColor.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;

TEST(UndirectedGraphColorTest, Empty)
{
    const auto graph = UndirectedGraphLibrary::Get_Null();
    const UndirectedGraphColor coloring(*graph);
    ASSERT_EQ(coloring.getChromaticNumber(), 0);
    ASSERT_ANY_THROW(coloring.getColoring2());
}

TEST(UndirectedGraphColorTest, Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const UndirectedGraphColor coloring(*graph);
    ASSERT_EQ(coloring.getChromaticNumber(), 1);
    ASSERT_ANY_THROW(coloring.getColoring2());
}

TEST(UndirectedGraphColorTest, Disconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const UndirectedGraphColor coloring(*graph);
    ASSERT_EQ(coloring.getChromaticNumber(), 1);
    ASSERT_ANY_THROW(coloring.getColoring2());
}

TEST(UndirectedGraphColorTest, Edge)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const UndirectedGraphColor coloring(*graph);
    ASSERT_EQ(coloring.getChromaticNumber(), 2);
    const auto color2 = coloring.getColoring2();
    ASSERT_EQ(color2.size(), 2);
    ASSERT_EQ(color2.at(0), 0);
    ASSERT_EQ(color2.at(1), 1);
}

TEST(UndirectedGraphColorTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const UndirectedGraphColor coloring(*graph);
    ASSERT_FALSE(coloring.getChromaticNumber().has_value());
}
