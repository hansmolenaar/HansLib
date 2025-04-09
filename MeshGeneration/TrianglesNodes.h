#pragma once

#include "CellsNodes.h"
#include "EdgeNodesSorted.h"
#include "TriangleNodesOriented.h"
#include <boost/container/static_vector.hpp>

namespace MeshGeneration
{
   class TrianglesNodes
   {
   public:
      CellIndex addTriangle(const Topology::TriangleNodesOriented& triangle);
      void deleteTriangle(CellIndex triangleId);
      boost::container::static_vector<CellIndex, 2> getEdgeConnectedTriangles(Topology::NodeIndex n0, Topology::NodeIndex n1) const;
      boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> getEdgeConnectedTriangles(CellIndex triangleId) const;
      boost::container::static_vector<CellIndex, Topology::NumNodesOnTriangle> getCommonNodes(CellIndex triangle1, CellIndex triangle2) const;
      std::vector<CellIndex> getNodeConnectedTriangles(Topology::NodeIndex node) const;
      std::vector<Topology::NodeIndex> getEdgeConnectedNodes(Topology::NodeIndex node) const;
      std::optional<CellIndex> tryGetTriangle(Topology::NodeIndex n0, Topology::NodeIndex n1, Topology::NodeIndex n2) const;
      bool triangleContainsNode(CellIndex CellIndex, Topology::NodeIndex nodeId) const;

      Topology::TriangleNodesOriented getTriangleNodes(CellIndex triangle) const;

      bool isKnownNodeId(Topology::NodeIndex node) const;
      bool isKnownTriangleId(CellIndex triangle) const;

      std::vector<CellIndex> getAllTriangles() const;
      std::vector<Topology::NodeIndex> getAllNodes() const;
      std::vector<Topology::EdgeNodesSorted> getAllSortedEdges() const;

      friend std::ostream& operator<<(std::ostream& os, const TrianglesNodes& tnodes)
      {
         const std::string sep = " ";
         os << "TriangleNodes";
         os << sep << "NUMNODES=" << tnodes.getAllNodes().size();
         os << sep + "NUMTRIANGLES=" << tnodes.m_cellsNodes.getNumCells();
         return os;
      }
      size_t getNumTriangles() const;

   private:
      CellsNodes<Topology::TriangleNodesOriented> m_cellsNodes;
   };
}