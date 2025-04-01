#include "Defines.h"
#include "MyException.h"
#include "TrianglesNodes.h"
#include <limits>

using namespace MeshGeneration;
using namespace Topology;

CellIndex TrianglesNodes::addTriangle(const TriangleNodesOriented& nodes)
{
   // Check for duplicates
   if (tryGetTriangleFromOrientedNodes(nodes))
   {
      std::ostringstream os;
      os << "TriangleNodes::addTriangle() triangle already exists: " << nodes;
      throw MyException(os.str());
   }

   // CellId must be unique to avoid an enormous mess
   const CellIndex result = m_newCellId;
   ++m_newCellId;
   m_toNodes.emplace(result, nodes);
   for (auto n : nodes)
   {
      m_toTriangles.emplace(n, result);
   }
   return result;
}

std::optional<CellIndex> TrianglesNodes::tryGetTriangleFromOrientedNodes(const TriangleNodesOriented& nodes) const
{
   const auto triangles = m_toTriangles.equal_range(nodes[0]);
   for (auto itr = triangles.first; itr != triangles.second; ++itr)
   {
      const CellIndex candidate = itr->second;
      const auto& candidateNodes = m_toNodes.at(candidate);
      if (str::equal(nodes, candidateNodes))
      {
         // Duplicates are not possible, already checked in addTriangle
         return candidate;
      }
   }
   return {};
}

std::optional<CellIndex> TrianglesNodes::tryGetTriangle(PointIndex n0, PointIndex n1, PointIndex n2) const
{
   checkNodeId(n0);
   checkNodeId(n1);
   checkNodeId(n2);
   const TriangleNodesOriented nodes(n0, n1, n2);
   return tryGetTriangleFromOrientedNodes(nodes);
}

void TrianglesNodes::deleteTriangle(CellIndex triangleId)
{
   if (m_toNodes.find(triangleId) == m_toNodes.end())
   {
      const std::string msg = "TriangleNodes::deleteTriangle() unkown triangle: " + std::to_string(triangleId);
      throw MyException(msg);
   }
   const auto& nodes = m_toNodes.at(triangleId);
   for (auto node : nodes)
   {
      const auto [first, last] = m_toTriangles.equal_range(node);
      bool found = false;
      for (auto itr = first; itr != last; ++itr)
      {
         if (itr->second == triangleId)
         {
            found = true;
            m_toTriangles.erase(itr);
            break;
         }
      }
      if (!found)
      {
         const std::string msg = "TriangleNodes::deleteTriangle() inconsistency in m_toTriangles: " + std::to_string(triangleId) + " " + std::to_string(node);
         throw MyException(msg);
      }
   }
   m_toNodes.erase(triangleId);
}

boost::container::static_vector<CellIndex, 2> TrianglesNodes::getEdgeConnectedTriangles(PointIndex n0, PointIndex n1) const
{
   boost::container::static_vector<CellIndex, 2> result;
   checkNodeId(n0);
   checkNodeId(n1);
   const auto [first, last] = m_toTriangles.equal_range(n0);
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

bool TrianglesNodes::triangleContainsNode(CellIndex triangleId, PointIndex nodeId) const
{
   checkTriangleId(triangleId);
   checkNodeId(nodeId);
   const auto& nodes = m_toNodes.at(triangleId);
   return str::find(nodes, nodeId) != nodes.end();
}

std::vector<CellIndex> TrianglesNodes::getNodeConnectedTriangles(PointIndex node) const
{
   std::vector<CellIndex> result;
   checkNodeId(node);
   const auto [first, last] = m_toTriangles.equal_range(node);
   for (auto itr = first; itr != last; ++itr)
   {
      result.push_back(itr->second);
   }
   str::sort(result);
   return result;
}

std::vector<PointIndex> TrianglesNodes::getEdgeConnectedNodes(PointIndex node) const
{
   std::vector<PointIndex> result;
   checkNodeId(node);
   const auto [first, last] = m_toTriangles.equal_range(node);
   for (auto itr = first; itr != last; ++itr)
   {
      const auto triangleId = itr->second;
      const auto& triangleNodes = m_toNodes.at(triangleId);
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

void TrianglesNodes::checkNodeId(PointIndex node) const
{
   if (!isKnownNodeId(node))
   {
      const std::string msg = "TriangleNodes::checkNodeId() unknown NodeId " + std::to_string(node);
      throw MyException(msg);
   }
}

void TrianglesNodes::checkTriangleId(CellIndex triangle) const
{
   if (!isKnownTriangleId(triangle))
   {
      const std::string msg = "TriangleNodes::checkTriangleId() unknown TriangleId " + std::to_string(triangle);
      throw MyException(msg);
   }
}

TriangleNodesOriented TrianglesNodes::getTriangleNodes(CellIndex triangle) const
{
   checkTriangleId(triangle);
   return m_toNodes.at(triangle);
}

bool TrianglesNodes::isKnownNodeId(PointIndex node) const
{
   return m_toTriangles.find(node) != m_toTriangles.end();
}

bool TrianglesNodes::isKnownTriangleId(CellIndex triangle) const
{
   return m_toNodes.find(triangle) != m_toNodes.end();
}

std::vector<CellIndex> TrianglesNodes::getAllTriangles() const
{
   std::vector<CellIndex> result;
   result.reserve(m_toNodes.size());
   for (auto& itr : m_toNodes)
   {
      result.push_back(itr.first);
   }
   str::sort(result);
   return result;
}

std::vector<EdgeNodesSorted> TrianglesNodes::getAllSortedEdges() const
{
   std::vector<EdgeNodesSorted> result;
   result.reserve(3 * m_toNodes.size());
   for (auto& itr : m_toNodes)
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

std::vector<PointIndex> TrianglesNodes::getAllNodes() const
{
   std::vector<PointIndex> result;
   for (auto iter = m_toTriangles.begin();
      iter != m_toTriangles.end();
      iter = m_toTriangles.equal_range(iter->first).second)
   {
      result.push_back(iter->first);
   }
   str::sort(result);
   return result;
}

std::string TrianglesNodes::toString() const
{
   const std::string sep = " ";
   std::string result = "TriangleNodes";
   result += sep + "NUMNODES=" + std::to_string(getAllNodes().size());
   result += sep + "NUMTRIANGLES=" + std::to_string(m_toNodes.size());
   return result;
}

size_t TrianglesNodes::getNumTriangles() const
{
   return m_toNodes.size();
}