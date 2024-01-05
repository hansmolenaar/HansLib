#include <gtest/gtest.h>

#include "TriangleNodes.h"
#include "MyException.h"

using namespace MeshGeneration;

TEST(TriangleNodesTest, Empty)
{
	TriangleNodes tnodes;
	ASSERT_ANY_THROW(tnodes.deleteTriangle(0));
	ASSERT_ANY_THROW(tnodes.getEdgeConnectedTriangles(0, 1));
	ASSERT_ANY_THROW(tnodes.getNodeConnectedTriangles(0));
	ASSERT_ANY_THROW(tnodes.getEdgeConnectedNodes(0));
	ASSERT_ANY_THROW(tnodes.tryGetTriangle(0, 1, 2));
	ASSERT_ANY_THROW(tnodes.triangleContainsNode(0, 1));
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

	//ASSERT_ANY_THROW(tnodes.tryGetTriangle(0, 1, 2));
	//ASSERT_ANY_THROW(tnodes.triangleContainsNode(0, 1));

	//ASSERT_ANY_THROW(tnodes.deleteTriangle(0));
}

