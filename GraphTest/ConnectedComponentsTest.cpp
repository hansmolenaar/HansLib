#include <gtest/gtest.h>

#include "UndirectedGraph.h"
#include "Defines.h"
TEST(ConnectedComponentsTest, Basics)
{
   UndirectedGraph ug(3);

   ASSERT_EQ(ug.getNumVertices(), 3);

   constexpr GraphVertex vertex2 = 2;
   ug.addEdge(1, vertex2);

   const auto components = ug.getConnectedComponents();
   ASSERT_EQ(*str::max_element(components), 1);
}
