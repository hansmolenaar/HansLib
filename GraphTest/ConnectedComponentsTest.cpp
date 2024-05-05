#include <gtest/gtest.h>

#include "GraphDefines.h"

TEST(ConnectedComponentsTest, Basics)
{
   UGraph ug(3);
   ASSERT_EQ(UGraphNumVertices(ug), 3);
   constexpr UGraphVertex vertex2 = 2;
   UGraphAddEdge(ug, 1, vertex2);

   const auto [components, numComponents] = UGraphGetConnectedComponents(ug);
   ASSERT_EQ(numComponents, 2);
}
