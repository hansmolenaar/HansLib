#pragma once

#include "EdgeNodesSorted.h"
#include "MeshGenerationDefines.h"
#include "TriangleNodesOriented.h"
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
      CellIndex addTriangle(const Topology::TriangleNodesOriented& triangle);
      void deleteTriangle(CellIndex triangleId);
      boost::container::static_vector<CellIndex, 2> getEdgeConnectedTriangles(PointIndex n0, PointIndex n1) const;
      boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> getEdgeConnectedTriangles(CellIndex triangleId) const;
      boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> getCommonNodes(CellIndex triangle1, CellIndex triangle2) const;
      std::vector<CellIndex> getNodeConnectedTriangles(PointIndex node) const;
      std::vector<PointIndex> getEdgeConnectedNodes(PointIndex node) const;
      std::optional<CellIndex> tryGetTriangle(PointIndex n0, PointIndex n1, PointIndex n2) const;
      bool triangleContainsNode(CellIndex CellIndex, PointIndex nodeId) const;

      Topology::TriangleNodesOriented getTriangleNodes(CellIndex triangle) const;

      bool isKnownNodeId(PointIndex node) const;
      bool isKnownTriangleId(CellIndex triangle) const;

      std::vector<CellIndex> getAllTriangles() const;
      std::vector<PointIndex> getAllNodes() const;
      std::vector<Topology::EdgeNodesSorted> getAllSortedEdges() const;

      friend std::ostream& operator<<(std::ostream& os, const TrianglesNodes& tnodes)
      {
         const std::string sep = " ";
         os << "TriangleNodes";
         os << sep << "NUMNODES=" << tnodes.getAllNodes().size();
         os << sep + "NUMTRIANGLES=" << tnodes.m_toNodes.size();
         return os;
      }
      size_t getNumTriangles() const;

   private:
      std::optional<CellIndex> tryGetTriangleFromOrientedNodes(const Topology::TriangleNodesOriented& nodes) const;
      void checkNodeId(PointIndex nodeId) const;
      void checkTriangleId(CellIndex triangleId) const;

      CellIndex m_newCellId = 0;
      std::unordered_map<CellIndex, Topology::TriangleNodesOriented> m_toNodes;
      std::unordered_multimap<PointIndex, CellIndex> m_toTriangles;
   };
}