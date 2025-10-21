#include <gtest/gtest.h>

#include "CellsNodes.h"
#include "MyException.h"
#include "Single.h"
#include "TriangleNodes.h"
#include "TrianglesNodes.h"

using namespace MeshGeneration;
using namespace Topology;
using namespace Utilities;

TEST(CellsNodesTest, Empty)
{
    CellsNodes<TriangleNodesOriented> tnodes;
    ASSERT_EQ(tnodes.getNumCells(), 0);
    ASSERT_TRUE(tnodes.getAllCellIds().empty());
    ASSERT_TRUE(tnodes.getAllNodes().empty());
    ASSERT_MYEXCEPTION_MESSAGE(tnodes.deleteCell(0), "CellsNodes<TCell>::checkCellId() unknown cellId 0");
    ASSERT_MYEXCEPTION_MESSAGE(tnodes.tryGetCellFromOrderedNodes(std::vector<NodeIndex>{0, 1, 2}),
                               "CellsNodes<TCell>::checkNodeId() unknown NodeId 0");
    ASSERT_MYEXCEPTION_MESSAGE(tnodes.cellContainsNode(0, 1), "CellsNodes<TCell>::checkCellId() unknown cellId 0");
    ASSERT_MYEXCEPTION_MESSAGE(tnodes.getCellNodes(0), "CellsNodes<TCell>::checkCellId() unknown cellId 0");
}

TEST(CellsNodesTest, SingleTriangle)
{
    CellsNodes<TriangleNodesOriented> tnodes;
    const auto triangleId = tnodes.addCell(TriangleNodesOriented(0, 42, 999));
    ASSERT_EQ(triangleId, 0);
    ASSERT_EQ(tnodes.getNumCells(), 1);

    ASSERT_EQ(tnodes.getAllCellIds().size(), 1);
    ASSERT_EQ(tnodes.getAllCellIds().front(), 0);
    ASSERT_EQ(tnodes.getAllNodes().size(), 3);
    ASSERT_EQ(tnodes.getAllNodes().back(), 999);

    const auto foundCell = tnodes.tryGetCellFromOrderedNodes(std::vector<NodeIndex>{0, 42, 999});
    ASSERT_EQ(triangleId, *foundCell);

    ASSERT_TRUE(tnodes.cellContainsNode(triangleId, 999));

    ASSERT_MYEXCEPTION_MESSAGE(tnodes.cellContainsNode(triangleId, 1),
                               "CellsNodes<TCell>::checkNodeId() unknown NodeId 1");

    const auto triangleNodes = tnodes.getCellNodes(triangleId);
    ASSERT_TRUE(str::equal(triangleNodes, std::array<PointIndex, 3>{0, 42, 999}));

    tnodes.deleteCell(triangleId);

    ASSERT_FALSE(tnodes.isKnownNodeId(42));
    ASSERT_FALSE(tnodes.isKnownNodeId(999));
    ASSERT_EQ(tnodes.getNumCells(), 0);
    ASSERT_TRUE(tnodes.getAllCellIds().empty());
    ASSERT_TRUE(tnodes.getAllNodes().empty());
}

TEST(CellsNodesTest, Delete)
{
    CellsNodes<TriangleNodesOriented> tnodes;
    const auto triangleId = tnodes.addCell(TriangleNodesOriented(42, 999, 0));
    tnodes.deleteCell(triangleId);
    ASSERT_FALSE(tnodes.isKnownNodeId(0));
    ASSERT_FALSE(tnodes.isKnownNodeId(42));
    ASSERT_FALSE(tnodes.isKnownNodeId(999));
    ASSERT_FALSE(tnodes.isKnownCellId(triangleId));
}

TEST(CellsNodesTest, IsKnown)
{
    CellsNodes<TriangleNodesOriented> tnodes;
    ASSERT_FALSE(tnodes.isKnownNodeId(0));
    ASSERT_FALSE(tnodes.isKnownCellId(0));

    const auto triangleId = tnodes.addCell(TriangleNodesOriented(42, 999, 6));

    ASSERT_TRUE(tnodes.isKnownNodeId(999));
    ASSERT_TRUE(tnodes.isKnownCellId(triangleId));
    ASSERT_FALSE(tnodes.isKnownNodeId(0));
    ASSERT_FALSE(tnodes.isKnownCellId(triangleId + 1));
}

TEST(CellsNodesTest, GetNodeConnectedTriangles)
{
    CellsNodes<TriangleNodes> tnodes;
    std::vector<CellIndex> cellIds;
    ASSERT_ANY_THROW(tnodes.getCellsContainingNodes(cellIds, std::vector<CellIndex>{0}));
    const auto triangle0 = tnodes.addCell(TriangleNodes(42, 999, 0));
    const auto triangle1 = tnodes.addCell(TriangleNodes(43, 999, 1));

    tnodes.getCellsContainingNodes(cellIds, std::vector<CellIndex>{1});
    ASSERT_EQ(Single(cellIds), triangle1);

    tnodes.getCellsContainingNodes(cellIds, std::vector<CellIndex>{999});
    ASSERT_TRUE(str::equal(cellIds, std::vector<CellIndex>{triangle0, triangle1}));
}

TEST(CellsNodesTest, GetEdgeConnectedNodes)
{
    CellsNodes<TriangleNodesOriented> tnodes;
    std::vector<NodeIndex> result;
    const auto triangle0 = tnodes.addCell(TriangleNodesOriented(42, 999, 0));
    const auto triangle1 = tnodes.addCell(TriangleNodesOriented(999, 42, 1));

    tnodes.getEdgeConnectedNodes(result, 1);
    ASSERT_TRUE(str::equal(result, std::vector<PointIndex>{42, 999}));

    tnodes.getEdgeConnectedNodes(result, 42);
    ASSERT_TRUE(str::equal(result, std::vector<PointIndex>{0, 1, 999}));
}

TEST(CellsNodesTest, GetEdgeConnectedTriangles)
{
    CellsNodes<TriangleNodesOriented> tnodes;

    const auto triangle0 = tnodes.addCell(TriangleNodesOriented(42, 999, 0));
    const auto triangle1 = tnodes.addCell(TriangleNodesOriented(999, 42, 1));

    std::vector<CellIndex> cellIds;
    tnodes.getCellsContainingNodes(cellIds, std::array<NodeIndex, 2>{1, 0});
    ASSERT_TRUE(cellIds.empty());

    tnodes.getCellsContainingNodes(cellIds, std::array<NodeIndex, 2>{1, 42});
    ASSERT_TRUE(str::equal(cellIds, std::vector<CellIndex>{triangle1}));

    tnodes.getCellsContainingNodes(cellIds, std::array<NodeIndex, 2>{999, 42});
    ASSERT_TRUE(str::equal(cellIds, std::vector<CellIndex>{triangle0, triangle1}));

    tnodes.getCellsContainingNodes(cellIds, std::array<NodeIndex, 2>{42, 999});
    ASSERT_TRUE(str::equal(cellIds, std::vector<CellIndex>{triangle0, triangle1}));
}

TEST(CellsNodesTest, TryGetTriangle)
{
    CellsNodes<TriangleNodesOriented> tnodes;
    std::vector<CellIndex> cellIds;
    const auto triangle0 = tnodes.addCell(TriangleNodesOriented(42, 999, 0));
    const auto triangle1 = tnodes.addCell(TriangleNodesOriented(999, 42, 1));

    const auto found_42_1_999 = tnodes.tryGetCellFromOrderedNodes(std::array<NodeIndex, 3>{1, 999, 42});
    ASSERT_EQ(triangle1, *found_42_1_999);

    const auto found_1_42_999 = tnodes.tryGetCellFromOrderedNodes(std::array<NodeIndex, 3>{1, 42, 999});
    ASSERT_FALSE(found_1_42_999);

    const auto found_0_1_42 = tnodes.tryGetCellFromOrderedNodes(std::array<NodeIndex, 3>{1, 42, 0});
    ASSERT_FALSE(found_0_1_42);
}

TEST(CellsNodesTest, TriangleContainsNode)
{
    CellsNodes<TriangleNodesOriented> tnodes;

    const auto triangle0 = tnodes.addCell(TriangleNodesOriented(42, 999, 0));
    const auto triangle1 = tnodes.addCell(TriangleNodesOriented(999, 42, 1));

    ASSERT_TRUE(tnodes.cellContainsNode(triangle1, 1));
    ASSERT_FALSE(tnodes.cellContainsNode(triangle1, 0));
}

TEST(CellsNodesTest, GetAllTriangles)
{
    CellsNodes<TriangleNodesOriented> tnodes;
    auto allTriangles = tnodes.getAllCellIds();
    ASSERT_TRUE(allTriangles.empty());
    ASSERT_EQ(tnodes.getNumCells(), allTriangles.size());

    const auto triangle0 = tnodes.addCell(TriangleNodesOriented(42, 999, 0));
    allTriangles = tnodes.getAllCellIds();
    ASSERT_TRUE(str::equal(allTriangles, std::vector<CellIndex>{triangle0}));
    ASSERT_EQ(tnodes.getNumCells(), allTriangles.size());

    const auto triangle1 = tnodes.addCell(TriangleNodesOriented(999, 42, 1));
    allTriangles = tnodes.getAllCellIds();
    ASSERT_TRUE(str::equal(allTriangles, std::vector<CellIndex>{triangle0, triangle1}));
    ASSERT_EQ(tnodes.getNumCells(), allTriangles.size());

    tnodes.deleteCell(triangle0);
    allTriangles = tnodes.getAllCellIds();
    ASSERT_TRUE(str::equal(allTriangles, std::vector<CellIndex>{triangle1}));
    ASSERT_EQ(tnodes.getNumCells(), allTriangles.size());

    tnodes.deleteCell(triangle1);
    allTriangles = tnodes.getAllCellIds();
    ASSERT_TRUE(allTriangles.empty());
    ASSERT_EQ(tnodes.getNumCells(), allTriangles.size());
}

TEST(CellsNodesTest, GetAllEdges)
{
    std::vector<EdgeNodesSorted> allEdges;
    CellsNodes<TriangleNodesOriented> tnodes;
    tnodes.getAlEdges(allEdges);
    ASSERT_TRUE(allEdges.empty());

    const auto triangle0 = tnodes.addCell(TriangleNodesOriented(42, 999, 0));
    tnodes.getAlEdges(allEdges);
    ASSERT_TRUE(str::equal(allEdges, std::vector<EdgeNodesSorted>{{0, 42}, {0, 999}, {42, 999}}));

    const auto triangle1 = tnodes.addCell(TriangleNodesOriented(42, 0, 2));
    tnodes.getAlEdges(allEdges);
    ASSERT_TRUE(str::equal(allEdges, std::vector<EdgeNodesSorted>{{0, 2}, {0, 42}, {0, 999}, {2, 42}, {42, 999}}));
}

TEST(CellsNodesTest, GetAllNodes)
{
    CellsNodes<TriangleNodesOriented> tnodes;
    const auto triangle0 = tnodes.addCell(TriangleNodesOriented(2, 1, 999));
    const auto triangle1 = tnodes.addCell(TriangleNodesOriented(1, 2, 42));
    const auto nodes = tnodes.getAllNodes();
    ASSERT_TRUE(str::equal(nodes, std::vector<NodeIndex>{1, 2, 42, 999}));
}

TEST(CellsNodesTest, GetCommonNodes)
{
    CellsNodes<TriangleNodesOriented> tnodes;
    const auto triangle0 = tnodes.addCell(TriangleNodesOriented(1, 2, 3));
    const auto triangle1 = tnodes.addCell(TriangleNodesOriented(6, 2, 1));
    const auto triangle2 = tnodes.addCell(TriangleNodesOriented(6, 5, 1));
    const auto triangle3 = tnodes.addCell(TriangleNodesOriented(6, 5, 4));
    std::vector<NodeIndex> result;
    auto actual = tnodes.getCommonNodes(triangle0, triangle0, result);
    std::vector expect{1, 2, 3};
    ASSERT_TRUE(str::equal(actual, expect));

    actual = tnodes.getCommonNodes(triangle0, triangle1, result);
    expect = {1, 2};
    ASSERT_TRUE(str::equal(actual, expect));

    actual = tnodes.getCommonNodes(triangle0, triangle2, result);
    expect = {1};
    ASSERT_TRUE(str::equal(actual, expect));

    actual = tnodes.getCommonNodes(triangle0, triangle3, result);
    ASSERT_TRUE(actual.empty());
}

TEST(CellsNodesTest, AddDelete)
{
    CellsNodes<TriangleNodesOriented> tnodes;
    const TriangleNodesOriented triangle{1, 2, 3};
    const auto triangleId0 = tnodes.addCell(triangle);
    tnodes.deleteCell(triangleId0);
    const auto triangleId1 = tnodes.addCell(triangle);
    ASSERT_NE(triangleId0, triangleId1);
}