#include <gtest/gtest.h>

#include "CellsNodes.h"
#include "MyException.h"
#include "TrianglesNodes.h"

using namespace MeshGeneration;
using namespace Topology;

TEST(CellsNodesAdd, Empty)
{
   CellsNodes<TriangleNodesOriented> tnodes;
   ASSERT_EQ(tnodes.getNumCells(), 0);
   ASSERT_TRUE(tnodes.getAllCellIds().empty());
   ASSERT_TRUE(tnodes.getAllNodes().empty());
   ASSERT_MYEXCEPTION_MESSAGE(tnodes.deleteCell(0), "CellsNodes<TCell>::checkCellId() unknown cellId 0");
   ASSERT_MYEXCEPTION_MESSAGE(tnodes.tryGetCell(std::vector<NodeIndex>{ 0, 1, 2 }), "CellsNodes<TCell>::checkNodeId() unknown NodeId 0");
   ASSERT_MYEXCEPTION_MESSAGE(tnodes.cellContainsNode(0, 1), "CellsNodes<TCell>::checkCellId() unknown cellId 0");
   ASSERT_MYEXCEPTION_MESSAGE(tnodes.getCellNodes(0), "CellsNodes<TCell>::checkCellId() unknown cellId 0");
}

TEST(CellsNodesAdd, SingleTriangle)
{
   CellsNodes<TriangleNodesOriented> tnodes;
   const auto triangleId = tnodes.addCell(TriangleNodesOriented(0, 42, 999));
   ASSERT_EQ(triangleId, 0);
   ASSERT_EQ(tnodes.getNumCells(), 1);

   ASSERT_EQ(tnodes.getAllCellIds().size(), 1);
   ASSERT_EQ(tnodes.getAllCellIds().front(), 0);
   ASSERT_EQ(tnodes.getAllNodes().size(), 3);
   ASSERT_EQ(tnodes.getAllNodes().back(), 999);

   const auto foundCell = tnodes.tryGetCell(std::vector<NodeIndex>{0, 42, 999});
   ASSERT_EQ(triangleId, *foundCell);

   ASSERT_TRUE(tnodes.cellContainsNode(triangleId, 999));

   ASSERT_MYEXCEPTION_MESSAGE(tnodes.cellContainsNode(triangleId, 1), "CellsNodes<TCell>::checkNodeId() unknown NodeId 1");

   const auto triangleNodes = tnodes.getCellNodes(triangleId);
   ASSERT_TRUE(str::equal(triangleNodes, std::array<PointIndex, 3>{0, 42, 999}));

   tnodes.deleteCell(triangleId);

   ASSERT_FALSE(tnodes.isKnownNodeId(42));
   ASSERT_FALSE(tnodes.isKnownNodeId(999));
   ASSERT_EQ(tnodes.getNumCells(), 0);
   ASSERT_TRUE(tnodes.getAllCellIds().empty());
   ASSERT_TRUE(tnodes.getAllNodes().empty());
}


TEST(CellsNodesAdd, Delete)
{
   CellsNodes<TriangleNodesOriented> tnodes;
   const auto triangleId = tnodes.addCell(TriangleNodesOriented(42, 999, 0));
   tnodes.deleteCell(triangleId);
   ASSERT_FALSE(tnodes.isKnownNodeId(0));
   ASSERT_FALSE(tnodes.isKnownNodeId(42));
   ASSERT_FALSE(tnodes.isKnownNodeId(999));
   ASSERT_FALSE(tnodes.isKnownCellId(triangleId));
}
#if false
TEST(CellsNodesAdd, IsKnown)
{
   TrianglesNodes tnodes;
   ASSERT_FALSE(tnodes.isKnownNodeId(0));
   ASSERT_FALSE(tnodes.isKnownTriangleId(0));

   const auto triangleId = tnodes.addTriangle(TriangleNodesOriented(42, 999, 6));

   ASSERT_TRUE(tnodes.isKnownNodeId(999));
   ASSERT_TRUE(tnodes.isKnownTriangleId(triangleId));
   ASSERT_FALSE(tnodes.isKnownNodeId(0));
   ASSERT_FALSE(tnodes.isKnownTriangleId(triangleId + 1));
}


TEST(CellsNodesAdd, GetNodeConnectedTriangles)
{
   TrianglesNodes tnodes;
   ASSERT_ANY_THROW(tnodes.getNodeConnectedTriangles(0));
   const auto triangle0 = tnodes.addTriangle(TriangleNodesOriented(42, 999, 0));
   const auto triangle1 = tnodes.addTriangle(TriangleNodesOriented(43, 999, 1));

   const auto found1 = tnodes.getNodeConnectedTriangles(1);
   ASSERT_TRUE(str::equal(found1, std::vector<CellIndex>{triangle1}));

   const auto found999 = tnodes.getNodeConnectedTriangles(999);
   ASSERT_TRUE(str::equal(found999, std::vector<CellIndex>{triangle0, triangle1}));
}


TEST(CellsNodesAdd, GetEdgeConnectedNodes)
{
   TrianglesNodes tnodes;

   const auto triangle0 = tnodes.addTriangle(TriangleNodesOriented(42, 999, 0));
   const auto triangle1 = tnodes.addTriangle(TriangleNodesOriented(999, 42, 1));

   const auto found_1 = tnodes.getEdgeConnectedNodes(1);
   ASSERT_TRUE(str::equal(found_1, std::vector<PointIndex>{42, 999}));

   const auto found_42 = tnodes.getEdgeConnectedNodes(42);
   ASSERT_TRUE(str::equal(found_42, std::vector<PointIndex>{0, 1, 999}));
}

TEST(CellsNodesAdd, GetEdgeConnectedTriangles)
{
   TrianglesNodes tnodes;

   const auto triangle0 = tnodes.addTriangle(TriangleNodesOriented(42, 999, 0));
   const auto triangle1 = tnodes.addTriangle(TriangleNodesOriented(999, 42, 1));

   const auto found_0_1 = tnodes.getEdgeConnectedTriangles(1, 0);
   ASSERT_TRUE(found_0_1.empty());

   const auto found_1_42 = tnodes.getEdgeConnectedTriangles(1, 42);
   ASSERT_TRUE(str::equal(found_1_42, std::vector<CellIndex>{triangle1}));

   const auto found_999_42 = tnodes.getEdgeConnectedTriangles(999, 42);
   ASSERT_TRUE(str::equal(found_999_42, std::vector<CellIndex>{triangle0, triangle1}));

   const auto found_42_999 = tnodes.getEdgeConnectedTriangles(42, 999);
   ASSERT_TRUE(str::equal(found_42_999, std::vector<CellIndex>{triangle0, triangle1}));
}


TEST(CellsNodesAdd, TryGetTriangle)
{
   TrianglesNodes tnodes;

   const auto triangle0 = tnodes.addTriangle(TriangleNodesOriented(42, 999, 0));
   const auto triangle1 = tnodes.addTriangle(TriangleNodesOriented(999, 42, 1));

   const auto found_42_1_999 = tnodes.tryGetTriangle(42, 1, 999);
   ASSERT_EQ(triangle1, *found_42_1_999);

   const auto found_1_42_999 = tnodes.tryGetTriangle(1, 42, 999);
   ASSERT_FALSE(found_1_42_999);

   const auto found_0_1_42 = tnodes.tryGetTriangle(1, 42, 0);
   ASSERT_FALSE(found_0_1_42);
}


TEST(CellsNodesAdd, TriangleContainsNode)
{
   TrianglesNodes tnodes;

   const auto triangle0 = tnodes.addTriangle(TriangleNodesOriented(42, 999, 0));
   const auto triangle1 = tnodes.addTriangle(TriangleNodesOriented(999, 42, 1));

   ASSERT_TRUE(tnodes.triangleContainsNode(triangle1, 1));
   ASSERT_FALSE(tnodes.triangleContainsNode(triangle1, 0));
}


TEST(CellsNodesAdd, GetAllTriangles)
{
   TrianglesNodes tnodes;
   auto allTriangles = tnodes.getAllTriangles();
   ASSERT_TRUE(allTriangles.empty());
   ASSERT_EQ(tnodes.getNumTriangles(), allTriangles.size());

   const auto triangle0 = tnodes.addTriangle(TriangleNodesOriented(42, 999, 0));
   allTriangles = tnodes.getAllTriangles();
   ASSERT_TRUE(str::equal(allTriangles, std::vector<CellIndex>{triangle0}));
   ASSERT_EQ(tnodes.getNumTriangles(), allTriangles.size());

   const auto triangle1 = tnodes.addTriangle(TriangleNodesOriented(999, 42, 1));
   allTriangles = tnodes.getAllTriangles();
   ASSERT_TRUE(str::equal(allTriangles, std::vector<CellIndex>{triangle0, triangle1}));
   ASSERT_EQ(tnodes.getNumTriangles(), allTriangles.size());

   tnodes.deleteTriangle(triangle0);
   allTriangles = tnodes.getAllTriangles();
   ASSERT_TRUE(str::equal(allTriangles, std::vector<CellIndex>{triangle1}));
   ASSERT_EQ(tnodes.getNumTriangles(), allTriangles.size());

   tnodes.deleteTriangle(triangle1);
   allTriangles = tnodes.getAllTriangles();
   ASSERT_TRUE(allTriangles.empty());
   ASSERT_EQ(tnodes.getNumTriangles(), allTriangles.size());
}


TEST(CellsNodesAdd, GetAllEdges)
{
   TrianglesNodes tnodes;
   auto allEdges = tnodes.getAllSortedEdges();
   ASSERT_TRUE(allEdges.empty());

   const auto triangle0 = tnodes.addTriangle(TriangleNodesOriented(42, 999, 0));
   allEdges = tnodes.getAllSortedEdges();
   ASSERT_TRUE(str::equal(allEdges, std::vector<EdgeNodesSorted>{EdgeNodesSorted{ 0, 42 }, EdgeNodesSorted{ 0,999 }, EdgeNodesSorted{ 42,999 }}));

   const auto triangle1 = tnodes.addTriangle(TriangleNodesOriented(42, 0, 2));
   allEdges = tnodes.getAllSortedEdges();
   ASSERT_TRUE(str::equal(allEdges, std::vector<EdgeNodesSorted>{{ 0, 2 }, { 0,42 }, { 0,999 }, { 2, 42 }, { 42, 999 }}));
}
TEST(CellsNodesAdd, ToString)
{
   std::ostringstream os;
   TrianglesNodes tnodes;
   os << tnodes;
   ASSERT_EQ(os.str(), "TriangleNodes NUMNODES=0 NUMTRIANGLES=0");

   const auto triangle0 = tnodes.addTriangle(TriangleNodesOriented(42, 999, 0));
   os.str("");
   os << tnodes;
   ASSERT_EQ(os.str(), "TriangleNodes NUMNODES=3 NUMTRIANGLES=1");

   const auto triangle1 = tnodes.addTriangle(TriangleNodesOriented(999, 42, 1));
   os.str("");
   os << tnodes;
   ASSERT_EQ(os.str(), "TriangleNodes NUMNODES=4 NUMTRIANGLES=2");
}
TEST(CellsNodesAdd, GetAllNodes)
{
   TrianglesNodes tnodes;
   const auto triangle0 = tnodes.addTriangle(TriangleNodesOriented(2, 1, 999));
   const auto triangle1 = tnodes.addTriangle(TriangleNodesOriented(1, 2, 42));
   const auto nodes = tnodes.getAllNodes();
   ASSERT_TRUE(str::equal(nodes, std::vector<NodeIndex>{1, 2, 42, 999}));
}

TEST(CellsNodesAdd, GetCommonNodes)
{
   TrianglesNodes tnodes;
   const auto triangle0 = tnodes.addTriangle(TriangleNodesOriented(1, 2, 3));
   const auto triangle1 = tnodes.addTriangle(TriangleNodesOriented(6, 2, 1));
   const auto triangle2 = tnodes.addTriangle(TriangleNodesOriented(6, 5, 1));
   const auto triangle3 = tnodes.addTriangle(TriangleNodesOriented(6, 5, 4));

   auto actual = tnodes.getCommonNodes(triangle0, triangle0);
   std::vector expect{ 1, 2, 3 };
   ASSERT_TRUE(str::equal(actual, expect));

   actual = tnodes.getCommonNodes(triangle0, triangle1);
   expect = { 1, 2 };
   ASSERT_TRUE(str::equal(actual, expect));

   actual = tnodes.getCommonNodes(triangle0, triangle2);
   expect = { 1 };
   ASSERT_TRUE(str::equal(actual, expect));

   actual = tnodes.getCommonNodes(triangle0, triangle3);
   ASSERT_TRUE(actual.empty());
}

TEST(CellsNodesAdd, AddDelete)
{
   TrianglesNodes tnodes;
   const TriangleNodesOriented triangle{ 1, 2, 3 };
   const auto triangleId0 = tnodes.addTriangle(triangle);
   tnodes.deleteTriangle(triangleId0);
   const auto triangleId1 = tnodes.addTriangle(triangle);
   ASSERT_NE(triangleId0, triangleId1);
}
#endif