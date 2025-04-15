#include "MyException.h"
#include "TrianglesNodes.h"
#include <sstream>

using namespace MeshGeneration;
using namespace Topology;

CellIndex TrianglesNodes::addTriangle(const TriangleNodesOriented& nodes)
{
   return m_cellsNodes.addCell(nodes);
}

std::optional<CellIndex> TrianglesNodes::tryGetTriangle(NodeIndex n0, NodeIndex n1, NodeIndex n2) const
{
   return m_cellsNodes.tryGetCellFromOrderedNodes(std::array<NodeIndex, NumNodesOnTriangle>{n0, n1, n2});
}

void TrianglesNodes::deleteTriangle(CellIndex triangleId)
{
   m_cellsNodes.deleteCell(triangleId);
}


boost::container::static_vector<CellIndex, 2> TrianglesNodes::getTrianglesContainingEdge(NodeIndex n0, NodeIndex n1) const
{
   boost::container::static_vector<CellIndex, 2> result;
   return m_cellsNodes.getCellsContainingNodes(result, std::array<NodeIndex, NumNodesOnEdge>{n0, n1});
}

boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> TrianglesNodes::getEdgeConnectedTriangles(CellIndex triangleId) const
{
   boost::container::static_vector<CellIndex, 3> result;
   const auto triangle = getTriangleNodes(triangleId);
   for (const auto& edge : triangle.getEdges())
   {
      const auto ngbs = getTrianglesContainingEdge(edge[0], edge[1]);
      if (ngbs.size() > 0 && ngbs[0] != triangleId) result.push_back(ngbs[0]);
      if (ngbs.size() > 1 && ngbs[1] != triangleId) result.push_back(ngbs[1]);
   }

   return result;
}

boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle>  TrianglesNodes::getCommonNodes(CellIndex triangle1, CellIndex triangle2) const
{
   boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> result;
   return m_cellsNodes.getCommonNodes(triangle1, triangle2, result);
}

bool TrianglesNodes::triangleContainsNode(CellIndex triangleId, NodeIndex nodeId) const
{
   return m_cellsNodes.cellContainsNode(triangleId, nodeId);
}


std::vector<CellIndex> TrianglesNodes::getTrianglesContainingNode(NodeIndex node) const
{
   std::vector<CellIndex> result;
   return m_cellsNodes.getCellsContainingNodes(result, std::array<NodeIndex, 1>{node});
}

std::vector<NodeIndex> TrianglesNodes::getEdgeConnectedNodes(NodeIndex node) const
{
   std::vector<NodeIndex> result;
   return m_cellsNodes.getEdgeConnectedNodes(result, node);
}


TriangleNodesOriented TrianglesNodes::getTriangleNodes(CellIndex triangle) const
{
   return m_cellsNodes.getCellNodes(triangle);
}

bool TrianglesNodes::isKnownNodeId(NodeIndex node) const
{
   return m_cellsNodes.isKnownNodeId(node);
}

bool TrianglesNodes::isKnownTriangleId(CellIndex triangle) const
{
   return m_cellsNodes.isKnownCellId(triangle);
}

std::vector<CellIndex> TrianglesNodes::getAllTriangles() const
{
   return m_cellsNodes.getAllCellIds();
}

std::vector<EdgeNodesSorted> TrianglesNodes::getAllSortedEdges() const
{
   std::vector<EdgeNodesSorted> result;
   return m_cellsNodes.getAlEdges(result);
}

std::vector<NodeIndex> TrianglesNodes::getAllNodes() const
{
   return m_cellsNodes.getAllNodes();
}

size_t TrianglesNodes::getNumTriangles() const
{
   return m_cellsNodes.getNumCells();
}