#include "TriangleNodes.h"
#include "Defines.h"
#include "MyException.h"
#include <limits>

using namespace MeshGeneration;

namespace
{
   // Check for duplicates, etc
   std::array<TriangleNodes::NodeId, TriangleNodes::NumNodesOnTriangle> SortAndCheckNodes(TriangleNodes::NodeId n0, TriangleNodes::NodeId n1, TriangleNodes::NodeId n2)
   {
      std::array<TriangleNodes::NodeId, TriangleNodes::NumNodesOnTriangle> nodes = { n0, n1, n2 };
      str::sort(nodes);
      const auto last = std::unique(nodes.begin(), nodes.end());
      if (last != nodes.end())
      {
         const std::string msg = "TriangleNodes::SortNodes: has duplicates: " + std::to_string(n0) + " " + std::to_string(n1) + " " + std::to_string(n2);
         throw MyException(msg);
      }
      return nodes;
   }

   TriangleNodes::SortedEdge CreateSortedEdge(TriangleNodes::NodeId n0, TriangleNodes::NodeId n1)
   {
      if (n1 > n0) return { n0, n1 };
      return { n1, n0 };
   }
}

TriangleNodes::TriangleId TriangleNodes::addTriangle(NodeId n0, NodeId n1, NodeId n2)
{
   const auto nodes = SortAndCheckNodes(n0, n1, n2);

   // Check for duplicates
   if (tryGetTriangleFromSortedNodes(nodes).first)
   {
      const std::string msg = "TriangleNodes::addTriangle() triangle already exists: " + std::to_string(n0) + " " + std::to_string(n1) + " " + std::to_string(n2);
      throw MyException(msg);
   }

   const TriangleId result = static_cast<TriangleId>(m_toNodes.size());
   m_toNodes.emplace(result, nodes);
   for (auto n : nodes)
   {
      m_toTriangles.emplace(n, result);
   }
   return result;
}

std::pair<bool, TriangleNodes::TriangleId> TriangleNodes::tryGetTriangleFromSortedNodes(const std::array<TriangleNodes::NodeId, TriangleNodes::NumNodesOnTriangle>& nodes) const
{
   std::pair<bool, TriangleNodes::TriangleId> result{ false,std::numeric_limits<TriangleNodes::TriangleId>::max() };
   const auto triangles = m_toTriangles.equal_range(nodes.at(0));
   for (auto itr = triangles.first; itr != triangles.second; ++itr)
   {
      const TriangleId candidate = itr->second;
      const auto& candidateNodes = m_toNodes.at(candidate);
      if (str::equal(nodes, candidateNodes))
      {
         // Duplicates are not possible, already checked in addTriangle
         return { true, candidate };
      }
   }
   return result;
}

std::pair<bool, TriangleNodes::TriangleId> TriangleNodes::tryGetTriangle(NodeId n0, NodeId n1, NodeId n2) const
{
   checkNodeId(n0);
   checkNodeId(n1);
   checkNodeId(n2);
   const auto nodes = SortAndCheckNodes(n0, n1, n2);
   return tryGetTriangleFromSortedNodes(nodes);
}

void TriangleNodes::deleteTriangle(TriangleId triangleId)
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

boost::container::static_vector<TriangleNodes::TriangleId, 2> TriangleNodes::getEdgeConnectedTriangles(TriangleNodes::NodeId n0, TriangleNodes::NodeId n1) const
{
   boost::container::static_vector<TriangleNodes::TriangleId, 2> result;
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

bool TriangleNodes::triangleContainsNode(TriangleId triangleId, NodeId nodeId) const
{
   checkTriangleId(triangleId);
   checkNodeId(nodeId);
   const auto& nodes = m_toNodes.at(triangleId);
   return str::find(nodes, nodeId) != nodes.end();
}

std::vector<TriangleNodes::TriangleId> TriangleNodes::getNodeConnectedTriangles(NodeId node) const
{
   std::vector<TriangleNodes::TriangleId> result;
   checkNodeId(node);
   const auto [first, last] = m_toTriangles.equal_range(node);
   for (auto itr = first; itr != last; ++itr)
   {
      result.push_back(itr->second);
   }
   str::sort(result);
   return result;
}

std::vector<TriangleNodes::NodeId> TriangleNodes::getEdgeConnectedNodes(NodeId node) const
{
   std::vector<TriangleNodes::NodeId> result;
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

void TriangleNodes::checkNodeId(NodeId node) const
{
   if (!isKnownNodeId(node))
   {
      const std::string msg = "TriangleNodes::checkNodeId() unknown NodeId " + std::to_string(node);
      throw MyException(msg);
   }
}

void TriangleNodes::checkTriangleId(TriangleId triangle) const
{
   if (!isKnownTriangleId(triangle))
   {
      const std::string msg = "TriangleNodes::checkTriangleId() unknown TriangleId " + std::to_string(triangle);
      throw MyException(msg);
   }
}

std::array<TriangleNodes::NodeId, TriangleNodes::NumNodesOnTriangle> TriangleNodes::getTriangleNodes(TriangleId triangle) const
{
   checkTriangleId(triangle);
   return m_toNodes.at(triangle);
}

bool TriangleNodes::isKnownNodeId(NodeId node) const
{
   return m_toTriangles.find(node) != m_toTriangles.end();
}

bool TriangleNodes::isKnownTriangleId(TriangleId triangle) const
{
   return m_toNodes.find(triangle) != m_toNodes.end();
}

std::vector<TriangleNodes::TriangleId> TriangleNodes::getAllTriangles() const
{
   std::vector<TriangleId> result;
   result.reserve(m_toNodes.size());
   for (auto& itr : m_toNodes)
   {
      result.push_back(itr.first);
   }
   str::sort(result);
   return result;
}

std::vector<TriangleNodes::SortedEdge> TriangleNodes::getAllSortedEdges() const
{
   std::vector<TriangleNodes::SortedEdge> result;
   result.reserve(3*m_toNodes.size());
   for (auto& itr : m_toNodes)
   {
      const auto& triangleNodes = itr.second;
      result.emplace_back(CreateSortedEdge(triangleNodes.at(0), triangleNodes.at(1)));
      result.emplace_back(CreateSortedEdge(triangleNodes.at(0), triangleNodes.at(2)));
      result.emplace_back(CreateSortedEdge(triangleNodes.at(1), triangleNodes.at(2)));
   }
   str::sort(result);
   const auto [first, last] = str::unique(result);
   result.erase(first, last);
   return result;
}