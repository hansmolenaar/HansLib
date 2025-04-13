#pragma once

#include "CellsNodes.h"
#include "EdgeNodesSorted.h"
#include "MeshGenerationDefines.h"
#include "TetrahedronNodesOriented.h"
#include "TopologyDefines.h"

#include <array>
#include <boost/container/static_vector.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace MeshGeneration
{
   class TetrahedronsNodes
   {
   public:
      CellIndex addTetrahedron(const Topology::TetrahedronNodesOriented& nodes);
      void deleteTetrahedron(CellIndex tetId);
      std::vector<CellIndex> getTetrahedronsContainingNode(Topology::NodeIndex node) const;
      std::vector<CellIndex> getTetrahedronsContainingEdge(Topology::NodeIndex n0, Topology::NodeIndex n1) const;
      //boost::container::static_vector<CellIndex, Topology::NumNodesOnTetrahedron> getEdgeConnectedTetrahedrons(CellIndex TetrahedronId) const;
      boost::container::static_vector<CellIndex, Topology::NumNodesOnTetrahedron> getCommonNodes(CellIndex tetId1, CellIndex tetId2) const;
      std::vector<Topology::NodeIndex> getEdgeConnectedNodes(Topology::NodeIndex node) const;
      std::optional<CellIndex> tryGetTetrahedronFromOrientedNodes(const std::array<Topology::NodeIndex, Topology::NumNodesOnTetrahedron>& nodes) const;
      std::optional<CellIndex> tryGetTetrahedronFromNodes(std::array<Topology::NodeIndex, Topology::NumNodesOnTetrahedron> nodes) const;
      bool tetrahedronContainsNode(CellIndex CellIndex, Topology::NodeIndex nodeId) const;

      Topology::TetrahedronNodesOriented getTetrahedronNodes(CellIndex tetId) const;

      bool isKnownNodeId(Topology::NodeIndex node) const;
      bool isKnownTetId(CellIndex tetId) const;

      std::vector<CellIndex> getAllTetrahedrons() const;
      std::vector<Topology::NodeIndex> getAllNodes() const;
      std::vector<Topology::EdgeNodesSorted> getAllSortedEdges() const;

      friend std::ostream& operator<<(std::ostream& os, const TetrahedronsNodes& tnodes)
      {
         const std::string sep = " ";
         os << "TetrahedronNodes";
         os << sep << "NUMNODES=" << tnodes.getAllNodes().size();
         os << sep + "NUMTETRAHEDRONS=" << tnodes.m_cellsNodes.getNumCells();
         return os;
      }
      size_t getNumTetrahedrons() const;
      std::vector<Topology::TriangleNodesOriented> getBoundaryFaces() const;

   private:
      CellsNodes<Topology::TetrahedronNodesOriented> m_cellsNodes;
   };
}