#include "Defines.h"
#include "MyException.h"
#include "TriangleNodesSorted.h"
#include "TrianglesNodes.h"
#include <limits>

using namespace MeshGeneration;
using namespace Topology;

CellIndex TrianglesNodes::addTriangle(PointIndex n0, PointIndex n1, PointIndex n2)
{
   const TriangleNodesSorted nodes(n0, n1, n2);

   // Check for duplicates
   if (tryGetTriangleFromSortedNodes(nodes))
   {
      const std::string msg = "TriangleNodes::addTriangle() triangle already exists: " + std::to_string(n0) + " " + std::to_string(n1) + " " + std::to_string(n2);
      throw MyException(msg);
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

CellIndex TrianglesNodes::addTriangle(const TriangleNodes& triangle)
{
   return addTriangle(triangle[0], triangle[1], triangle[2]);
}

std::optional<CellIndex> TrianglesNodes::tryGetTriangleFromSortedNodes(const TriangleNodesSorted& nodes) const
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
   const TriangleNodesSorted nodes(n0, n1, n2);
   return tryGetTriangleFromSortedNodes(nodes);
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
   for (size_t n = 0; n < Topology::NumNodesOnTriangle; ++n)
   {
      const auto ngbs = getEdgeConnectedTriangles(triangle[n], triangle[(n + 1) % Topology::NumNodesOnTriangle]);
      if (ngbs.size() > 0 && ngbs[0] != triangleId) result.push_back(ngbs[0]);
      if (ngbs.size() > 1 && ngbs[1] != triangleId) result.push_back(ngbs[1]);
   }

   return result;
}

boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle>  TrianglesNodes::getCommonNodes(CellIndex triangle1, CellIndex triangle2) const
{
   boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> result;
   const auto trianglNodes1 = getTriangleNodes(triangle1);
   const auto trianglNodes2 = getTriangleNodes(triangle2);
   str::set_intersection(trianglNodes1, trianglNodes2, std::back_inserter(result));
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

TriangleNodesSorted TrianglesNodes::getTriangleNodes(CellIndex triangle) const
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