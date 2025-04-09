#pragma once

#include "EdgeNodesSorted.h"
#include "MeshGenerationDefines.h"
#include "TetrahedronNodesOriented.h"
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
      //boost::container::static_vector<CellIndex, 2> getEdgeConnectedTetrahedrons(Topology::NodeIndex n0, Topology::NodeIndex n1) const;
      //boost::container::static_vector<CellIndex, Topology::NumNodesOnTetrahedron> getEdgeConnectedTetrahedrons(CellIndex TetrahedronId) const;
      //boost::container::static_vector<CellIndex, Topology::NumNodesOnTetrahedron> getCommonNodes(CellIndex Tetrahedron1, CellIndex Tetrahedron2) const;
      //std::vector<CellIndex> getNodeConnectedTetrahedrons(Topology::NodeIndex node) const;
      //std::vector<Topology::NodeIndex> getEdgeConnectedNodes(Topology::NodeIndex node) const;
      std::optional<CellIndex> tryGetTetrahedron(Topology::NodeIndex n0, Topology::NodeIndex n1, Topology::NodeIndex n2, Topology::NodeIndex n3) const;
      bool TetrahedronContainsNode(CellIndex CellIndex, Topology::NodeIndex nodeId) const;

      Topology::TetrahedronNodesOriented getTetrahedronNodes(CellIndex tetId) const;

      bool isKnownNodeId(Topology::NodeIndex node) const;
      bool isKnownTetrahedronId(CellIndex tetId) const;

      std::vector<CellIndex> getAllTetrahedrons() const;
      std::vector<Topology::NodeIndex> getAllNodes() const;
      std::vector<Topology::EdgeNodesSorted> getAllSortedEdges() const;

      friend std::ostream& operator<<(std::ostream& os, const TetrahedronsNodes& tnodes)
      {
         const std::string sep = " ";
         os << "TetrahedronNodes";
         os << sep << "NUMNODES=" << tnodes.getAllNodes().size();
         os << sep + "NUMTETRAHEDRONS=" << tnodes.m_toNodes.size();
         return os;
      }
      size_t getNumTetrahedrons() const;

   private:
      std::optional<CellIndex> tryGetTetrahedronFromNodes(const Topology::TetrahedronNodesOriented& nodes) const;
      void checkNodeId(Topology::NodeIndex nodeId) const;
      void checkTetrahedronId(CellIndex tetId) const;

      CellIndex m_newCellId = 0;
      std::unordered_map<CellIndex, Topology::TetrahedronNodesOriented> m_toNodes;
      std::unordered_multimap<Topology::NodeIndex, CellIndex> m_toTetrahedrons;
   };
}