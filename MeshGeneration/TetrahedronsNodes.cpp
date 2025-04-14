#include "Defines.h"
#include "MyException.h"
#include "TetrahedronsNodes.h"
#include <limits>
#include <set>

using namespace MeshGeneration;
using namespace Topology;

CellIndex TetrahedronsNodes::addTetrahedron(const TetrahedronNodesOriented& nodes)
{
   return m_cellsNodes.addCell(nodes);
}
#if false
std::optional<CellIndex> TetrahedronsNodes::tryGetTetrahedronFromNodes(const TetrahedronNodesOriented& nodes) const
{
   const auto tetrahedrons = m_toTetrahedrons.equal_range(nodes[0]);
   for (auto itr = tetrahedrons.first; itr != tetrahedrons.second; ++itr)
   {
      const CellIndex candidate = itr->second;
      const auto& candidateNodes = m_toNodes.at(candidate);
      if (str::equal(nodes, candidateNodes))
      {
         // Duplicates are not possible, already checked in addTetrahedron
         return candidate;
      }
   }
   return {};
}
#endif

std::optional<CellIndex> TetrahedronsNodes::tryGetTetrahedronFromOrientedNodes(const std::array<NodeIndex, NumNodesOnTetrahedron>& nodes) const
{
   return m_cellsNodes.tryGetCell(nodes);
}

std::optional<CellIndex> TetrahedronsNodes::tryGetTetrahedronFromNodes(std::array<NodeIndex, NumNodesOnTetrahedron> nodes) const
{
   return tryGetTetrahedronFromOrientedNodes(TetrahedronNodesOriented::orient(nodes));
}

void TetrahedronsNodes::deleteTetrahedron(CellIndex tetId)
{
   m_cellsNodes.deleteCell(tetId);
}

std::vector<CellIndex> TetrahedronsNodes::getTetrahedronsContainingEdge(Topology::NodeIndex n0, Topology::NodeIndex n1) const
{
   std::vector<CellIndex> result;
   return m_cellsNodes.getCellsContainingNodes(result, std::array<NodeIndex, NumNodesOnEdge>{n0, n1});
}

std::vector<CellIndex> TetrahedronsNodes::getTetrahedronsContainingNode(NodeIndex node) const
{
   std::vector<CellIndex> result;
   return m_cellsNodes.getCellsContainingNodes(result, std::array<NodeIndex, 1>{node});
}

std::vector<Topology::NodeIndex> TetrahedronsNodes::getEdgeConnectedNodes(Topology::NodeIndex node) const
{
   std::vector<Topology::NodeIndex> result;
   return m_cellsNodes.getEdgeConnectedNodes(result, node);
}

#if false
boost::container::static_vector<CellIndex, Topology::NumNodesOnTetrahedron> TetrahedronsNodes::getEdgeConnectedTetrahedrons(CellIndex tetId) const
{
   boost::container::static_vector<CellIndex, 3> result;
   const auto tetrahedron = getTetrahedronNodes(tetId);
   for (const auto& edge : tetrahedron.getEdges())
   {
      const auto ngbs = getEdgeConnectedTetrahedrons(edge[0], edge[1]);
      if (ngbs.size() > 0 && ngbs[0] != tetId) result.push_back(ngbs[0]);
      if (ngbs.size() > 1 && ngbs[1] != tetId) result.push_back(ngbs[1]);
   }

   return result;
}
#endif

boost::container::static_vector<CellIndex, Topology::NumNodesOnTetrahedron>  TetrahedronsNodes::getCommonNodes(CellIndex tetId1, CellIndex tetId2) const
{
   boost::container::static_vector<CellIndex, Topology::NumNodesOnTetrahedron> result;
   return m_cellsNodes.getCommonNodes(tetId1, tetId2, result);
}

bool TetrahedronsNodes::tetrahedronContainsNode(CellIndex tetId, NodeIndex nodeId) const
{
   return m_cellsNodes.cellContainsNode(tetId, nodeId);
}
#if false
std::vector<CellIndex> TetrahedronsNodes::getNodeConnectedTetrahedrons(NodeIndex node) const
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
      const auto tetId = itr->second;
      const auto& tetrahedronNodes = m_toNodes.at(tetId);
      for (auto ngb : tetrahedronNodes)
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

#endif

TetrahedronNodesOriented TetrahedronsNodes::getTetrahedronNodes(CellIndex tetId) const
{
   return m_cellsNodes.getCellNodes(tetId);
}

bool TetrahedronsNodes::isKnownNodeId(NodeIndex node) const
{
   return m_cellsNodes.isKnownNodeId(node);
}

bool TetrahedronsNodes::isKnownTetId(CellIndex tetId) const
{
   return m_cellsNodes.isKnownCellId(tetId);
}

std::vector<CellIndex> TetrahedronsNodes::getAllTetrahedrons() const
{
   return m_cellsNodes.getAllCellIds();
}

std::vector<EdgeNodesSorted> TetrahedronsNodes::getAllSortedEdges() const
{
   std::vector<EdgeNodesSorted> result;
   return m_cellsNodes.getAlEdges(result);
}

std::vector<NodeIndex> TetrahedronsNodes::getAllNodes() const
{
   return m_cellsNodes.getAllNodes();
}

size_t TetrahedronsNodes::getNumTetrahedrons() const
{
   return m_cellsNodes.getNumCells();
}


TrianglesNodes TetrahedronsNodes::getBoundaryFaces() const
{
   std::set<TriangleNodesOriented> uniqueFaces;
   for (CellIndex tetId : getAllTetrahedrons())
   {
      const auto tet = getTetrahedronNodes(tetId);
      for (auto face : tet.getFaces())
      {
         const auto retval = uniqueFaces.insert(face);
         if (!retval.second)
         {
            throw MyException("getBoundaryFaces overconnected face: ");
         }
      }
   }

   TrianglesNodes result;
   for (const auto& face : uniqueFaces)
   {
      if (!uniqueFaces.contains(TriangleNodesOriented(face[1], face[0], face[2])))
      {
         result.addTriangle(face);
      }
   }
   return result;
}