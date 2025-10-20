#include <gtest/gtest.h>

#include "Boundary1.h"
#include "Defines.h"
#include "EdgeNodesSorted.h"
#include "MyException.h"
#include "TetrahedronsNodes.h"

using namespace MeshGeneration;
using namespace Topology;

TEST(TetrahedronsNodesTest, EmptyTetCollectionB)
{
    TetrahedronsNodes tnodes;
    ASSERT_EQ(tnodes.getNumTetrahedrons(), 0);
    ASSERT_TRUE(tnodes.getAllTetrahedrons().empty());
    ASSERT_TRUE(tnodes.getAllNodes().empty());
    ASSERT_EQ(tnodes.getBoundaryFaces().getNumTriangles(), 0);
    ASSERT_MYEXCEPTION_MESSAGE(tnodes.deleteTetrahedron(0), "CellsNodes<TCell>::checkCellId() unknown cellId 0");
    // ASSERT_MYEXCEPTION_MESSAGE(tnodes.getTetrahedronsContainingEdge(0, 1), "CellsNodes<TCell>::checkNodeId() unknown
    // NodeId 0"); ASSERT_MYEXCEPTION_MESSAGE(tnodes.getNodeConnectedTetrahedrons(0), "CellsNodes<TCell>::checkNodeId()
    // unknown NodeId 0"); ASSERT_MYEXCEPTION_MESSAGE(tnodes.getEdgeConnectedNodes(0), "CellsNodes<TCell>::checkNodeId()
    // unknown NodeId 0"); ASSERT_MYEXCEPTION_MESSAGE(tnodes.tryGetTetrahedron(0, 1, 2),
    // "CellsNodes<TCell>::checkNodeId() unknown NodeId 0");
    ASSERT_MYEXCEPTION_MESSAGE(tnodes.tetrahedronContainsNode(0, 1),
                               "CellsNodes<TCell>::checkCellId() unknown cellId 0");
    ASSERT_MYEXCEPTION_MESSAGE(tnodes.getTetrahedronNodes(0), "CellsNodes<TCell>::checkCellId() unknown cellId 0");
}

TEST(TetrahedronsNodesTest, SingleTet)
{
    TetrahedronsNodes tnodes;
    const auto tetId = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 0, 2));
    ASSERT_EQ(tnodes.getNumTetrahedrons(), 1);

    const auto connectedTetrahedrons = tnodes.getTetrahedronsContainingEdge(0, 42);
    ASSERT_EQ(1, connectedTetrahedrons.size());
    ASSERT_EQ(tetId, connectedTetrahedrons.at(0));

    const auto nodeConnectedTetrahedrons = tnodes.getTetrahedronsContainingNode(999);
    ASSERT_EQ(1, nodeConnectedTetrahedrons.size());
    ASSERT_EQ(tetId, nodeConnectedTetrahedrons.at(0));

    const auto edgeConnectedNodes = tnodes.getEdgeConnectedNodes(0);
    ASSERT_EQ(3, edgeConnectedNodes.size());
    ASSERT_EQ(2, edgeConnectedNodes.at(0));
    ASSERT_EQ(42, edgeConnectedNodes.at(1));
    ASSERT_EQ(999, edgeConnectedNodes.at(2));

    auto foundTet = tnodes.tryGetTetrahedronFromOrientedNodes({0, 2, 42, 999});
    ASSERT_EQ(tetId, *foundTet);
    foundTet = tnodes.tryGetTetrahedronFromOrientedNodes({0, 42, 2, 999});
    ASSERT_FALSE(foundTet);

    ASSERT_TRUE(tnodes.tetrahedronContainsNode(tetId, 999));
    ASSERT_MYEXCEPTION_MESSAGE(tnodes.tetrahedronContainsNode(tetId, 1),
                               "CellsNodes<TCell>::checkNodeId() unknown NodeId 1");

    const auto tetNodes = tnodes.getTetrahedronNodes(tetId);
    ASSERT_TRUE(str::equal(tetNodes, std::array<NodeIndex, 4>{0, 2, 42, 999}));

    const auto boundaryFaces = tnodes.getBoundaryFaces();
    ASSERT_EQ(boundaryFaces.getNumTriangles(), NumFacesOnTetrahedron);

    tnodes.deleteTetrahedron(tetId);
    ASSERT_FALSE(tnodes.isKnownNodeId(42));
    ASSERT_FALSE(tnodes.isKnownNodeId(999));
    ASSERT_EQ(tnodes.getNumTetrahedrons(), 0);
}

TEST(TetrahedronsNodesTest, DeleteTetrahedron)
{
    TetrahedronsNodes tnodes;
    const auto tetId = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 0, 2));
    tnodes.deleteTetrahedron(tetId);
    ASSERT_FALSE(tnodes.isKnownNodeId(0));
    ASSERT_FALSE(tnodes.isKnownNodeId(42));
    ASSERT_FALSE(tnodes.isKnownNodeId(999));
    ASSERT_FALSE(tnodes.isKnownTetId(tetId));
}

TEST(TetrahedronsNodesTest, IsKnownTetrahedron)
{
    TetrahedronsNodes tnodes;
    ASSERT_FALSE(tnodes.isKnownNodeId(0));
    ASSERT_FALSE(tnodes.isKnownTetId(0));

    const auto tetrahedronId = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 6, 1));

    ASSERT_TRUE(tnodes.isKnownNodeId(999));
    ASSERT_TRUE(tnodes.isKnownTetId(tetrahedronId));
    ASSERT_FALSE(tnodes.isKnownNodeId(0));
    ASSERT_FALSE(tnodes.isKnownTetId(tetrahedronId + 1));
}

TEST(TetrahedronsNodesTest, GetNodeConnectedTetrahedrons)
{
    TetrahedronsNodes tnodes;
    ASSERT_ANY_THROW(tnodes.getTetrahedronsContainingNode(0));
    const auto tet0 = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 0, 2));
    const auto tet1 = tnodes.addTetrahedron(TetrahedronNodesOriented(43, 999, 1, 4));

    const auto found1 = tnodes.getTetrahedronsContainingNode(1);
    ASSERT_TRUE(str::equal(found1, std::vector<CellIndex>{tet1}));

    const auto found999 = tnodes.getTetrahedronsContainingNode(999);
    ASSERT_TRUE(str::equal(found999, std::vector<CellIndex>{tet0, tet1}));
}

TEST(TetrahedronsNodesTest, GetEdgeConnectedNodes)
{
    TetrahedronsNodes tnodes;

    const auto tetId0 = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 0, 2));
    const auto tetId1 = tnodes.addTetrahedron(TetrahedronNodesOriented(999, 42, 1, 3));

    const auto found_1 = tnodes.getEdgeConnectedNodes(1);
    ASSERT_TRUE(str::equal(found_1, std::vector<NodeIndex>{3, 42, 999}));

    const auto found_42 = tnodes.getEdgeConnectedNodes(42);
    ASSERT_TRUE(str::equal(found_42, std::vector<NodeIndex>{0, 1, 2, 3, 999}));
}

TEST(TetrahedronsNodesTest, GetEdgeConnectedTetrahedrons)
{
    TetrahedronsNodes tnodes;

    const auto tetId0 = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 0, 2));
    const auto tetId1 = tnodes.addTetrahedron(TetrahedronNodesOriented(999, 42, 1, 3));

    const auto found_0_1 = tnodes.getTetrahedronsContainingEdge(1, 0);
    ASSERT_TRUE(found_0_1.empty());

    const auto found_1_42 = tnodes.getTetrahedronsContainingEdge(1, 42);
    ASSERT_TRUE(str::equal(found_1_42, std::vector<CellIndex>{tetId1}));

    const auto found_999_42 = tnodes.getTetrahedronsContainingEdge(999, 42);
    ASSERT_TRUE(str::equal(found_999_42, std::vector<CellIndex>{tetId0, tetId1}));

    const auto found_42_999 = tnodes.getTetrahedronsContainingEdge(42, 999);
    ASSERT_TRUE(str::equal(found_42_999, std::vector<CellIndex>{tetId0, tetId1}));
}

TEST(TetrahedronsNodesTest, tryGetTetrahedronFromOrientedNodes)
{
    TetrahedronsNodes tnodes;

    const auto tetId0 = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 0, 4));
    const auto tetId1 = tnodes.addTetrahedron(TetrahedronNodesOriented(999, 42, 1, 3));

    auto found = tnodes.tryGetTetrahedronFromOrientedNodes({1, 3, 999, 42});
    ASSERT_FALSE(found);
    found = tnodes.tryGetTetrahedronFromNodes({1, 3, 999, 42});
    ASSERT_TRUE(found);
    ASSERT_EQ(tetId1, *found);
    found = tnodes.tryGetTetrahedronFromOrientedNodes(TetrahedronNodesOriented::orient({1, 3, 999, 42}));
    ASSERT_EQ(tetId1, *found);

    found = tnodes.tryGetTetrahedronFromOrientedNodes({1, 3, 42, 999});
    ASSERT_FALSE(found);

    found = tnodes.tryGetTetrahedronFromOrientedNodes({1, 42, 0, 4});
    ASSERT_FALSE(found);
}

TEST(TetrahedronsNodesTest, TetrahedronContainsNode)
{
    TetrahedronsNodes tnodes;

    const auto tetId0 = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 0, 3));
    const auto tetId1 = tnodes.addTetrahedron(TetrahedronNodesOriented(999, 42, 1, 3));

    ASSERT_TRUE(tnodes.tetrahedronContainsNode(tetId1, 1));
    ASSERT_FALSE(tnodes.tetrahedronContainsNode(tetId1, 0));
}

TEST(TetrahedronsNodesTest, GetAllTetrahedrons)
{
    TetrahedronsNodes tnodes;
    auto allTetrahedrons = tnodes.getAllTetrahedrons();
    ASSERT_TRUE(allTetrahedrons.empty());
    ASSERT_EQ(tnodes.getNumTetrahedrons(), allTetrahedrons.size());

    const auto tetId0 = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 0, 2));
    allTetrahedrons = tnodes.getAllTetrahedrons();
    ASSERT_TRUE(str::equal(allTetrahedrons, std::vector<CellIndex>{tetId0}));
    ASSERT_EQ(tnodes.getNumTetrahedrons(), allTetrahedrons.size());

    const auto tetId1 = tnodes.addTetrahedron(TetrahedronNodesOriented(999, 42, 1, 2));
    allTetrahedrons = tnodes.getAllTetrahedrons();
    ASSERT_TRUE(str::equal(allTetrahedrons, std::vector<CellIndex>{tetId0, tetId1}));
    ASSERT_EQ(tnodes.getNumTetrahedrons(), allTetrahedrons.size());

    tnodes.deleteTetrahedron(tetId0);
    allTetrahedrons = tnodes.getAllTetrahedrons();
    ASSERT_TRUE(str::equal(allTetrahedrons, std::vector<CellIndex>{tetId1}));
    ASSERT_EQ(tnodes.getNumTetrahedrons(), allTetrahedrons.size());

    tnodes.deleteTetrahedron(tetId1);
    allTetrahedrons = tnodes.getAllTetrahedrons();
    ASSERT_TRUE(allTetrahedrons.empty());
    ASSERT_EQ(tnodes.getNumTetrahedrons(), allTetrahedrons.size());
}

TEST(TetrahedronsNodesTest, GetAllEdges)
{
    TetrahedronsNodes tnodes;
    auto allEdges = tnodes.getAllSortedEdges();
    ASSERT_TRUE(allEdges.empty());

    const auto tetId0 = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 0, 1));
    allEdges = tnodes.getAllSortedEdges();
    const std::vector<EdgeNodesSorted> expect{EdgeNodesSorted(0, 1),   EdgeNodesSorted(0, 42),
                                              EdgeNodesSorted(0, 999), EdgeNodesSorted(1, 42),
                                              EdgeNodesSorted(1, 999), EdgeNodesSorted(42, 999)};
    ASSERT_TRUE(str::equal(allEdges, expect));

    const auto tetId1 = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 3, 5, 1));
    allEdges = tnodes.getAllSortedEdges();
    ASSERT_EQ(allEdges.size(), 11);
}

TEST(TetrahedronsNodesTest, TwoTets)
{
    TetrahedronsNodes tnodes;
    const auto tetId1 = tnodes.addTetrahedron(TetrahedronNodesOriented(1, 2, 3, 4));
    const auto tetId2 = tnodes.addTetrahedron(TetrahedronNodesOriented(1, 3, 2, 5));
    ASSERT_EQ(tnodes.getNumTetrahedrons(), 2);

    const auto boundaryFaces = tnodes.getBoundaryFaces();
    ASSERT_EQ(boundaryFaces.getNumTriangles(), 6);
    const auto boundary = Boundary1::createFromBoundaryEdges(boundaryFaces);
    ASSERT_TRUE(boundary.empty());
}

TEST(TetrahedronsNodesTest, ToString)
{
    std::ostringstream os;
    TetrahedronsNodes tnodes;
    os << tnodes;
    ASSERT_EQ(os.str(), "TetrahedronNodes NUMNODES=0 NUMTETRAHEDRONS=0");

    const auto tetId0 = tnodes.addTetrahedron(TetrahedronNodesOriented(42, 999, 0, 2));
    os.str("");
    os << tnodes;
    ASSERT_EQ(os.str(), "TetrahedronNodes NUMNODES=4 NUMTETRAHEDRONS=1");

    const auto tetId1 = tnodes.addTetrahedron(TetrahedronNodesOriented(999, 42, 1, 3));
    os.str("");
    os << tnodes;
    ASSERT_EQ(os.str(), "TetrahedronNodes NUMNODES=6 NUMTETRAHEDRONS=2");
}

TEST(TetrahedronsNodesTest, GetAllNodes)
{
    TetrahedronsNodes tnodes;
    const auto tetId0 = tnodes.addTetrahedron(TetrahedronNodesOriented(2, 1, 999, 3));
    const auto tetId1 = tnodes.addTetrahedron(TetrahedronNodesOriented(1, 2, 4, 42));
    const auto nodes = tnodes.getAllNodes();
    ASSERT_TRUE(str::equal(nodes, std::vector<NodeIndex>{1, 2, 3, 4, 42, 999}));
}

TEST(TetrahedronsNodesTest, GetCommonNodes)
{
    TetrahedronsNodes tnodes;
    const auto tetId0 = tnodes.addTetrahedron(TetrahedronNodesOriented(1, 2, 3, 4));
    const auto tetId1 = tnodes.addTetrahedron(TetrahedronNodesOriented(6, 2, 1, 5));
    const auto tetId2 = tnodes.addTetrahedron(TetrahedronNodesOriented(6, 5, 1, 7));
    const auto tetId3 = tnodes.addTetrahedron(TetrahedronNodesOriented(6, 5, 7, 8));

    auto actual = tnodes.getCommonNodes(tetId0, tetId0);
    std::vector expect{1, 2, 3, 4};
    ASSERT_TRUE(str::equal(actual, expect));

    actual = tnodes.getCommonNodes(tetId0, tetId1);
    expect = {1, 2};
    ASSERT_TRUE(str::equal(actual, expect));

    actual = tnodes.getCommonNodes(tetId0, tetId2);
    expect = {1};
    ASSERT_TRUE(str::equal(actual, expect));

    actual = tnodes.getCommonNodes(tetId0, tetId3);
    ASSERT_TRUE(actual.empty());
}

TEST(TetrahedronsNodesTest, AddDeleteAdd)
{
    TetrahedronsNodes tnodes;
    const TetrahedronNodesOriented tetrahedron{1, 2, 3, 4};
    const auto tetrahedronId0 = tnodes.addTetrahedron(tetrahedron);
    tnodes.deleteTetrahedron(tetrahedronId0);
    const auto tetrahedronId1 = tnodes.addTetrahedron(tetrahedron);
    ASSERT_NE(tetrahedronId0, tetrahedronId1);
}