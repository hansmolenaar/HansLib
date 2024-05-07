#include "UndirectedGraphLibrary.h"
#include "MyAssert.h"

using namespace Utilities;

namespace
{
	static void SetEdgesOfPath(UndirectedGraph& graph, GraphEdge numEdges)
	{
		for (GraphEdge edge = 0; edge < numEdges; ++edge)
		{
			graph.addEdge(edge, edge + 1);
		}
	}
}

std::unique_ptr<UndirectedGraph> UndirectedGraphLibrary::Get_Path(GraphVertex numVertices)
{
	MyAssert(numVertices > 1);
	auto result = std::make_unique<UndirectedGraph>(numVertices);
	SetEdgesOfPath(*result, numVertices - 1);
	return result;
}