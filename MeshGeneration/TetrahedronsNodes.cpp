#include "Defines.h"
#include "MyException.h"
#include "TetrahedronsNodes.h"
#include <limits>

using namespace MeshGeneration;
using namespace Topology;

CellIndex TetrahedronsNodes::addTetrahedron(const TetrahedronNodesOriented& nodes)
{
   // Check for duplicates
   if (tryGetTetrahedronFromNodes(nodes))
   {
      std::ostringstream os;
      os << "TetrahedronsNodes::addTriangle() triangle already exists: " << nodes;
      throw MyException(os.str());
   }

   // CellId must be unique to avoid an enormous mess
   const CellIndex result = m_newCellId;
   ++m_newCellId;
   m_toNodes.emplace(result, nodes);
   for (auto n : nodes)
   {
      m_toTetrahedrons.emplace(n, result);
   }
   return result;
}

std::optional<CellIndex> TetrahedronsNodes::tryGetTetrahedronFromNodes(const TetrahedronNodesOriented& nodes) const
{
   const auto tetrahedrons = m_toTetrahedrons.equal_range(nodes[0]);
   for (auto itr = tetrahedrons.first; itr != tetrahedrons.second; ++itr)
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


std::optional<CellIndex> TetrahedronsNodes::tryGetTetrahedron(NodeIndex n0, NodeIndex n1, NodeIndex n2, NodeIndex n3) const
{
   checkNodeId(n0);
   checkNodeId(n1);
   checkNodeId(n2);
   checkNodeId(n3);
   const TetrahedronNodesOriented nodes(n0, n1, n2, n3);
   return tryGetTetrahedronFromNodes(nodes);
}
#if false
void TetrahedronsNodes::deleteTriangle(CellIndex triangleId)
{
   if (m_toNodes.find(triangleId) == m_toNodes.end())
   {
      const std::string msg = "TriangleNodes::deleteTriangle() unkown triangle: " + std::to_string(triangleId);
      throw MyException(msg);
   }
   const auto& nodes = m_toNodes.at(triangleId);
   for (auto node : nodes)
   {
      const auto [first, last] = m_toTetrahedrons.equal_range(node);
      bool found = false;
      for (auto itr = first; itr != last; ++itr)
      {
         if (itr->second == triangleId)
         {
            found = true;
            m_toTetrahedrons.erase(itr);
            break;
         }
      }
      if (!found)
      {
         const std::string msg = "TriangleNodes::deleteTriangle() inconsistency in m_toTetrahedrons: " + std::to_string(triangleId) + " " + std::to_string(node);
         throw MyException(msg);
      }
   }
   m_toNodes.erase(triangleId);
}

boost::container::static_vector<CellIndex, 2> TetrahedronsNodes::getEdgeConnectedTriangles(NodeIndex n0, NodeIndex n1) const
{
   boost::container::static_vector<CellIndex, 2> result;
   checkNodeId(n0);
   checkNodeId(n1);
   const auto [first, last] = m_toTetrahedrons.equal_range(n0);
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

boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> TetrahedronsNodes::getEdgeConnectedTriangles(CellIndex triangleId) const
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

boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle>  TetrahedronsNodes::getCommonNodes(CellIndex triangle1, CellIndex triangle2) const
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

bool TetrahedronsNodes::triangleContainsNode(CellIndex triangleId, NodeIndex nodeId) const
{
   checkTriangleId(triangleId);
   checkNodeId(nodeId);
   const auto& nodes = m_toNodes.at(triangleId);
   return str::find(nodes, nodeId) != nodes.end();
}

std::vector<CellIndex> TetrahedronsNodes::getNodeConnectedTriangles(NodeIndex node) const
{
   std::vector<CellIndex> result;
   checkNodeId(node);
   const auto [first, last] = m_toTetrahedrons.equal_range(node);
   for (auto itr = first; itr != last; ++itr)
   {
      result.push_back(itr->second);
   }
   str::sort(result);
   return result;
}

std::vector<NodeIndex> TetrahedronsNodes::getEdgeConnectedNodes(NodeIndex node) const
{
   std::vector<NodeIndex> result;
   checkNodeId(node);
   const auto [first, last] = m_toTetrahedrons.equal_range(node);
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

void TetrahedronsNodes::checkNodeId(NodeIndex node) const
{
   if (!isKnownNodeId(node))
   {
      const std::string msg = "TriangleNodes::checkNodeId() unknown NodeId " + std::to_string(node);
      throw MyException(msg);
   }
}

void TetrahedronsNodes::checkTriangleId(CellIndex triangle) const
{
   if (!isKnownTriangleId(triangle))
   {
      const std::string msg = "TriangleNodes::checkTriangleId() unknown TriangleId " + std::to_string(triangle);
      throw MyException(msg);
   }
}

TriangleNodesOriented TetrahedronsNodes::getTriangleNodes(CellIndex triangle) const
{
   checkTriangleId(triangle);
   return m_toNodes.at(triangle);
}

bool TetrahedronsNodes::isKnownNodeId(NodeIndex node) const
{
   return m_toTetrahedrons.find(node) != m_toTetrahedrons.end();
}

bool TetrahedronsNodes::isKnownTriangleId(CellIndex triangle) const
{
   return m_toNodes.find(triangle) != m_toNodes.end();
}

std::vector<CellIndex> TetrahedronsNodes::getAllTriangles() const
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

std::vector<EdgeNodesSorted> TetrahedronsNodes::getAllSortedEdges() const
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

std::vector<NodeIndex> TetrahedronsNodes::getAllNodes() const
{
   std::vector<NodeIndex> result;
   for (auto iter = m_toTetrahedrons.begin();
      iter != m_toTetrahedrons.end();
      iter = m_toTetrahedrons.equal_range(iter->first).second)
   {
      result.push_back(iter->first);
   }
   str::sort(result);
   return result;
}

size_t TetrahedronsNodes::getNumTriangles() const
{
   return m_toNodes.size();
}
#endif