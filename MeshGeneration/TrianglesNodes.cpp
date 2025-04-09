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
   return m_cellsNodes.tryGetCell(std::array<NodeIndex, NumNodesOnTriangle>{n0, n1, n2});
}

void TrianglesNodes::deleteTriangle(CellIndex triangleId)
{
   m_cellsNodes.deleteCell(triangleId);
}


boost::container::static_vector<CellIndex, 2> TrianglesNodes::getEdgeConnectedTriangles(NodeIndex n0, NodeIndex n1) const
{
   boost::container::static_vector<CellIndex, 2> result;
   m_cellsNodes.checkNodeId(n0);
   m_cellsNodes.checkNodeId(n1);
   const auto [first, last] = m_cellsNodes.getNodeToCellIds().equal_range(n0);
   for (auto itr = first; itr != last; ++itr)
   {
      if (triangleContainsNode(itr->second, n1))
      {
         result.push_back(itr->second);
      }
   }
   str::sort(result);
   return result;
}

boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> TrianglesNodes::getEdgeConnectedTriangles(CellIndex triangleId) const
{
   boost::container::static_vector<CellIndex, 3> result;
   const auto triangle = getTriangleNodes(triangleId);
   for (const auto& edge : triangle.getEdges())
   {
      const auto ngbs = getEdgeConnectedTriangles(edge[0], edge[1]);
      if (ngbs.size() > 0 && ngbs[0] != triangleId) result.push_back(ngbs[0]);
      if (ngbs.size() > 1 && ngbs[1] != triangleId) result.push_back(ngbs[1]);
   }

   return result;
}

boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle>  TrianglesNodes::getCommonNodes(CellIndex triangle1, CellIndex triangle2) const
{
   boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> result;
   const auto triangleNodes1 = getTriangleNodes(triangle1);
   const auto triangleNodes2 = getTriangleNodes(triangle2);
   for (auto node : triangleNodes1)
   {
      if (triangleNodes2.contains(node)) result.push_back(node);
   }
   return result;
}

bool TrianglesNodes::triangleContainsNode(CellIndex triangleId, NodeIndex nodeId) const
{
   return m_cellsNodes.cellContainsNode(triangleId, nodeId);
}


std::vector<CellIndex> TrianglesNodes::getNodeConnectedTriangles(NodeIndex node) const
{
   std::vector<CellIndex> result;
   m_cellsNodes.checkNodeId(node);
   const auto [first, last] = m_cellsNodes.getNodeToCellIds().equal_range(node);
   for (auto itr = first; itr != last; ++itr)
   {
      result.push_back(itr->second);
   }
   str::sort(result);
   return result;
}

std::vector<NodeIndex> TrianglesNodes::getEdgeConnectedNodes(NodeIndex node) const
{
   std::vector<NodeIndex> result;
   m_cellsNodes.checkNodeId(node);
   const auto [first, last] = m_cellsNodes.getNodeToCellIds().equal_range(node);
   const auto& toNodes = m_cellsNodes.getCellIdToNodes();
   for (auto itr = first; itr != last; ++itr)
   {
      const auto triangleId = itr->second;
      const auto& triangleNodes = toNodes.at(triangleId);
      for (auto ngb : triangleNodes)
      {
         if ((ngb != node) && (str::find(result, ngb) == result.end()))
         {
            result.push_back(ngb);
         }
      }
   }
   str::sort(result);
   return result;
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
   const auto& toNodes = m_cellsNodes.getCellIdToNodes();
   result.reserve(3 * toNodes.size());
   for (auto& itr : toNodes)
   {
      const auto& triangleNodes = itr.second;
      result.emplace_back(triangleNodes.at(0), triangleNodes.at(1));
      result.emplace_back(triangleNodes.at(0), triangleNodes.at(2));
      result.emplace_back(triangleNodes.at(1), triangleNodes.at(2));
   }
   str::sort(result);
   const auto [first, last] = str::unique(result);
   result.erase(first, last);
   return result;
}

std::vector<NodeIndex> TrianglesNodes::getAllNodes() const
{
   return m_cellsNodes.getAllNodes();
}

size_t TrianglesNodes::getNumTriangles() const
{
   return m_cellsNodes.getNumCells();
}