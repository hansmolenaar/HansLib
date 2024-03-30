#pragma once

#include "Defines.h"
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
      static constexpr size_t NumNodesOnEdge = 2;
      static constexpr size_t NumNodesOnTriangle = 3;

      using SortedEdge = std::array<PointIndex, NumNodesOnEdge>;

      // No ordering of the nodes assumed
      CellIndex addTriangle(PointIndex n0, PointIndex n1, PointIndex n2);
      void deleteTriangle(CellIndex triangleId);
      boost::container::static_vector<CellIndex, 2> getEdgeConnectedTriangles(PointIndex n0, PointIndex n1) const;
      std::vector<CellIndex> getNodeConnectedTriangles(PointIndex node) const;
      std::vector<PointIndex> getEdgeConnectedNodes(PointIndex node) const;
      std::optional<CellIndex> tryGetTriangle(PointIndex n0, PointIndex n1, PointIndex n2) const;
      bool triangleContainsNode(CellIndex CellIndex, PointIndex nodeId) const;

      std::array<PointIndex, TrianglesNodes::NumNodesOnTriangle> getTriangleNodes(CellIndex triangle) const;

      bool isKnownNodeId(PointIndex node) const;
      bool isKnownTriangleId(CellIndex triangle) const;

      std::vector<CellIndex> getAllTriangles() const;
      std::vector<PointIndex> getAllNodes() const;
      std::vector<TrianglesNodes::SortedEdge> getAllSortedEdges() const;

      std::string toString() const;

   private:
      std::optional<CellIndex> tryGetTriangleFromSortedNodes(const std::array<PointIndex, NumNodesOnTriangle>& nodes) const;
      void checkNodeId(PointIndex nodeId) const;
      void checkTriangleId(CellIndex triangleId) const;

      std::unordered_map<CellIndex, std::array<PointIndex, NumNodesOnTriangle>> m_toNodes;
      std::unordered_multimap<PointIndex, CellIndex> m_toTriangles;
   };
}