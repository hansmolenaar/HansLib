#include <gtest/gtest.h>

#include "TriangleNodes.h"
#include "MyException.h"
#include "Defines.h"

using namespace MeshGeneration;

TEST(TriangleNodesTest, Empty)
{
   TriangleNodes tnodes;
   ASSERT_THROW(tnodes.deleteTriangle(0), MyException);
   ASSERT_THROW(tnodes.getEdgeConnectedTriangles(0, 1), MyException);
   ASSERT_THROW(tnodes.getNodeConnectedTriangles(0), MyException);
   ASSERT_THROW(tnodes.getEdgeConnectedNodes(0), MyException);
   ASSERT_THROW(tnodes.tryGetTriangle(0, 1, 2), MyException);
   ASSERT_THROW(tnodes.triangleContainsNode(0, 1), MyException);
   ASSERT_THROW(tnodes.getTriangleNodes(0), MyException);
}

TEST(TriangleNodesTest, SingleTriangle)
{
   TriangleNodes tnodes;
   const auto triangleId = tnodes.addTriangle(42, 999, 0);

   const auto connectedTriangles = tnodes.getEdgeConnectedTriangles(0, 42);
   ASSERT_EQ(1, connectedTriangles.size());
   ASSERT_EQ(triangleId, connectedTriangles.at(0));

   const auto nodeConnectedTriangles = tnodes.getNodeConnectedTriangles(999);
   ASSERT_EQ(1, nodeConnectedTriangles.size());
   ASSERT_EQ(triangleId, nodeConnectedTriangles.at(0));

   const auto edgeConnectedNodes = tnodes.getEdgeConnectedNodes(0);
   ASSERT_EQ(2, edgeConnectedNodes.size());
   ASSERT_EQ(42, edgeConnectedNodes.at(0));
   ASSERT_EQ(999, edgeConnectedNodes.at(1));

   const auto foundTriangle = tnodes.tryGetTriangle(0, 42, 999);
   ASSERT_TRUE(foundTriangle.first);
   ASSERT_EQ(triangleId, foundTriangle.second);

   ASSERT_TRUE(tnodes.triangleContainsNode(triangleId, 999));
   ASSERT_THROW(tnodes.triangleContainsNode(triangleId, 1), MyException);

   const auto triangleNodes = tnodes.getTriangleNodes(triangleId);
   ASSERT_TRUE(str::equal(triangleNodes, std::array<TriangleNodes::NodeId, 3>{0, 42, 999}));

   tnodes.deleteTriangle(triangleId);
   ASSERT_FALSE(tnodes.isKnownNodeId(42));
   ASSERT_FALSE(tnodes.isKnownNodeId(999));
}

TEST(TriangleNodesTest, Delete)
{
   TriangleNodes tnodes;
   const auto triangleId = tnodes.addTriangle(42, 999, 0);
   tnodes.deleteTriangle(triangleId);
   ASSERT_FALSE(tnodes.isKnownNodeId(0));
   ASSERT_FALSE(tnodes.isKnownNodeId(42));
   ASSERT_FALSE(tnodes.isKnownNodeId(999));
   ASSERT_FALSE(tnodes.isKnownTriangleId(triangleId));
}

TEST(TriangleNodesTest, IsKnown)
{
   TriangleNodes tnodes;
   ASSERT_FALSE(tnodes.isKnownNodeId(0));
   ASSERT_FALSE(tnodes.isKnownTriangleId(0));

   const auto triangleId = tnodes.addTriangle(42, 999, 6);

   ASSERT_TRUE(tnodes.isKnownNodeId(999));
   ASSERT_TRUE(tnodes.isKnownTriangleId(triangleId));
   ASSERT_FALSE(tnodes.isKnownNodeId(0));
   ASSERT_FALSE(tnodes.isKnownTriangleId(triangleId + 1));
}


TEST(TriangleNodesTest, GetNodeConnectedTriangles)
{
   TriangleNodes tnodes;
   ASSERT_ANY_THROW(tnodes.getNodeConnectedTriangles(0));
   const auto triangle0 = tnodes.addTriangle(42, 999, 0);
   const auto triangle1 = tnodes.addTriangle(43, 999, 1);

   const auto found1 = tnodes.getNodeConnectedTriangles(1);
   ASSERT_TRUE(str::equal(found1, std::vector<TriangleNodes::TriangleId>{triangle1}));

   const auto found999 = tnodes.getNodeConnectedTriangles(999);
   ASSERT_TRUE(str::equal(found999, std::vector<TriangleNodes::TriangleId>{triangle0, triangle1}));
}


TEST(TriangleNodesTest, GetEdgeConnectedNodes)
{
   TriangleNodes tnodes;

   const auto triangle0 = tnodes.addTriangle(42, 999, 0);
   const auto triangle1 = tnodes.addTriangle(999, 42, 1);

   const auto found_1 = tnodes.getEdgeConnectedNodes(1);
   ASSERT_TRUE(str::equal(found_1, std::vector<TriangleNodes::NodeId>{42, 999}));

   const auto found_42 = tnodes.getEdgeConnectedNodes(42);
   ASSERT_TRUE(str::equal(found_42, std::vector<TriangleNodes::NodeId>{0, 1, 999}));
}

TEST(TriangleNodesTest, GetEdgeConnectedTriangles)
{
   TriangleNodes tnodes;

   const auto triangle0 = tnodes.addTriangle(42, 999, 0);
   const auto triangle1 = tnodes.addTriangle(999, 42, 1);

   const auto found_0_1 = tnodes.getEdgeConnectedTriangles(1, 0);
   ASSERT_TRUE(found_0_1.empty());

   const auto found_1_42 = tnodes.getEdgeConnectedTriangles(1, 42);
   ASSERT_TRUE(str::equal(found_1_42, std::vector<TriangleNodes::TriangleId>{triangle1}));

   const auto found_999_42 = tnodes.getEdgeConnectedTriangles(999, 42);
   ASSERT_TRUE(str::equal(found_999_42, std::vector<TriangleNodes::TriangleId>{triangle0, triangle1}));

   const auto found_42_999 = tnodes.getEdgeConnectedTriangles(42, 999);
   ASSERT_TRUE(str::equal(found_42_999, std::vector<TriangleNodes::TriangleId>{triangle0, triangle1}));
}


TEST(TriangleNodesTest, TryGetTriangle)
{
   TriangleNodes tnodes;

   const auto triangle0 = tnodes.addTriangle(42, 999, 0);
   const auto triangle1 = tnodes.addTriangle(999, 42, 1);

   const auto found_1_42_999 = tnodes.tryGetTriangle(1, 42, 999);
   ASSERT_TRUE(found_1_42_999.first);
   ASSERT_EQ(triangle1, found_1_42_999.second);

   const auto found_0_1_42 = tnodes.tryGetTriangle(1, 42, 0);
   ASSERT_FALSE(found_0_1_42.first);
}


TEST(TriangleNodesTest, TriangleContainsNode)
{
   TriangleNodes tnodes;

   const auto triangle0 = tnodes.addTriangle(42, 999, 0);
   const auto triangle1 = tnodes.addTriangle(999, 42, 1);

   ASSERT_TRUE(tnodes.triangleContainsNode(triangle1, 1));
   ASSERT_FALSE(tnodes.triangleContainsNode(triangle1, 0));
}


TEST(TriangleNodesTest, GetAllTriangles)
{
   TriangleNodes tnodes;
   auto allTriangles = tnodes.getAllTriangles();
   ASSERT_TRUE(allTriangles.empty());

   const auto triangle0 = tnodes.addTriangle(42, 999, 0);
   allTriangles = tnodes.getAllTriangles();
   ASSERT_TRUE(str::equal(allTriangles, std::vector<TriangleNodes::TriangleId>{triangle0}));

   const auto triangle1 = tnodes.addTriangle(999, 42, 1);
   allTriangles = tnodes.getAllTriangles();
   ASSERT_TRUE(str::equal(allTriangles, std::vector<TriangleNodes::TriangleId>{triangle0, triangle1}));

   tnodes.deleteTriangle(triangle0);
   allTriangles = tnodes.getAllTriangles();
   ASSERT_TRUE(str::equal(allTriangles, std::vector<TriangleNodes::TriangleId>{triangle1}));

   tnodes.deleteTriangle(triangle1);
   allTriangles = tnodes.getAllTriangles();
   ASSERT_TRUE(allTriangles.empty());
}


TEST(TriangleNodesTest, GetAllEdges)
{
   using Edge = TriangleNodes::SortedEdge;
   TriangleNodes tnodes;
   auto allEdges = tnodes.getAllSortedEdges();
   ASSERT_TRUE(allEdges.empty());

   const auto triangle0 = tnodes.addTriangle(42, 999, 0);
   allEdges = tnodes.getAllSortedEdges();
   ASSERT_TRUE(str::equal(allEdges, std::vector<Edge>{Edge{ 0, 42 }, Edge{ 0,999 }, Edge{ 42,999 }}));

   const auto triangle1 = tnodes.addTriangle(42, 0, 2);
   allEdges = tnodes.getAllSortedEdges();
   ASSERT_TRUE(str::equal(allEdges, std::vector<Edge>{Edge{ 0, 2 }, Edge{ 0,42 }, Edge{ 0,999 }, Edge{ 2, 42 }, Edge{ 42, 999 }}));
}