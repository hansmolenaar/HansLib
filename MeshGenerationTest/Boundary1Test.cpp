#include <gtest/gtest.h>

#include "Boundary1.h"
#include "Single.h"
#include "TrianglesNodes.h"

using namespace MeshGeneration;
using namespace Topology;
using namespace Utilities;

TEST(Boundary1Test, singleEdge)
{
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(TriangleNodesOriented(0, 1, 2));

   Boundary1 boundary1(std::vector<NodeIndex>{2, 1}, trianglesNodes);
   ASSERT_TRUE(boundary1.getSingletons().empty());
   ASSERT_TRUE(boundary1.getCycles().empty());
   ASSERT_EQ(boundary1.getPaths().size(), 1);
   ASSERT_TRUE(str::equal(boundary1.getPaths().front(), std::vector<NodeIndex>{2, 1}));
}

TEST(Boundary1Test, singleCycle)
{
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(TriangleNodesOriented(0, 1, 2));

   Boundary1 boundary1(std::vector<NodeIndex>{1, 0, 2}, trianglesNodes);
   ASSERT_TRUE(boundary1.getSingletons().empty());
   ASSERT_TRUE(boundary1.getPaths().empty());
   ASSERT_EQ(boundary1.getCycles().size(), 1);
   ASSERT_TRUE(str::equal(boundary1.getCycles().front(), std::vector<NodeIndex>{1, 0, 2}));
}

TEST(Boundary1Test, singleNode)
{
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(TriangleNodesOriented(0, 1, 2));

   Boundary1 boundary1(std::vector<NodeIndex>{1}, trianglesNodes);
   ASSERT_EQ(Single(boundary1.getSingletons()), 1);
   ASSERT_TRUE(boundary1.getPaths().empty());
   ASSERT_TRUE(boundary1.getCycles().empty());
}

TEST(Boundary1Test, empty)
{
   Boundary1 boundary;
   ASSERT_TRUE(boundary.empty());
   boundary = Boundary1::createSingleCycleForTesting({ 1,2,3 });
   ASSERT_FALSE(boundary.empty());
}
