#pragma once

#include "EdgeNodesSorted.h"
#include "MeshGenerationDefines.h"
#include "TriangleNodes.h"
#include <array>
#include <boost/container/static_vector.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace MeshGeneration
{
   class TrianglesNodes
   {
   public:


      // No ordering of the nodes assumed
      CellIndex addTriangle(PointIndex n0, PointIndex n1, PointIndex n2);
      void deleteTriangle(CellIndex triangleId);
      boost::container::static_vector<CellIndex, 2> getEdgeConnectedTriangles(PointIndex n0, PointIndex n1) const;
      boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> getEdgeConnectedTriangles(CellIndex triangleId) const;
      std::vector<CellIndex> getNodeConnectedTriangles(PointIndex node) const;
      std::vector<PointIndex> getEdgeConnectedNodes(PointIndex node) const;
      std::optional<CellIndex> tryGetTriangle(PointIndex n0, PointIndex n1, PointIndex n2) const;
      bool triangleContainsNode(CellIndex CellIndex, PointIndex nodeId) const;

      Topology::TriangleNodes getTriangleNodes(CellIndex triangle) const;

      bool isKnownNodeId(PointIndex node) const;
      bool isKnownTriangleId(CellIndex triangle) const;

      std::vector<CellIndex> getAllTriangles() const;
      std::vector<PointIndex> getAllNodes() const;
      std::vector<Topology::EdgeNodesSorted> getAllSortedEdges() const;

      std::string toString() const;

   private:
      std::optional<CellIndex> tryGetTriangleFromSortedNodes(const Topology::TriangleNodes& nodes) const;
      void checkNodeId(PointIndex nodeId) const;
      void checkTriangleId(CellIndex triangleId) const;

      std::unordered_map<CellIndex, Topology::TriangleNodes> m_toNodes;
      std::unordered_multimap<PointIndex, CellIndex> m_toTriangles;
   };
}