#pragma once

#include "MeshGenerationDefines.h"
#include <boost/container/static_vector.hpp>
#include <vector>
#include <unordered_map>
#include <array>
#include <optional>
#include <string>

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

      TriangleNodes getTriangleNodes(CellIndex triangle) const;

      bool isKnownNodeId(PointIndex node) const;
      bool isKnownTriangleId(CellIndex triangle) const;

      std::vector<CellIndex> getAllTriangles() const;
      std::vector<PointIndex> getAllNodes() const;
      std::vector<SortedEdgeNodes> getAllSortedEdges() const;

      std::string toString() const;
      static SortedEdgeNodes CreateSortedEdge(PointIndex n0, PointIndex n1);

   private:
      std::optional<CellIndex> tryGetTriangleFromSortedNodes(const TriangleNodes& nodes) const;
      void checkNodeId(PointIndex nodeId) const;
      void checkTriangleId(CellIndex triangleId) const;

      std::unordered_map<CellIndex, TriangleNodes> m_toNodes;
      std::unordered_multimap<PointIndex, CellIndex> m_toTriangles;
   };
}