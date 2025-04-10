#include "Defines.h"
#include "MyException.h"
#include "TetrahedronsNodes.h"
#include <limits>

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

std::optional<CellIndex> TetrahedronsNodes::tryGetTetrahedron(NodeIndex n0, NodeIndex n1, NodeIndex n2, NodeIndex n3) const
{
   return m_cellsNodes.tryGetCell(std::array<NodeIndex, NumNodesOnTetrehadron>{n0, n1, n2, n3});
}


void TetrahedronsNodes::deleteTetrahedron(CellIndex tetId)
{
   m_cellsNodes.deleteCell(tetId);
}
#if false
boost::container::static_vector<CellIndex, 2> TetrahedronsNodes::getEdgeConnectedTetrahedrons(NodeIndex n0, NodeIndex n1) const
{
   boost::container::static_vector<CellIndex, 2> result;
   checkNodeId(n0);
   checkNodeId(n1);
   const auto [first, last] = m_toTetrahedrons.equal_range(n0);
   for (auto itr = first; itr != last; ++itr)
   {
      if (tetrahedronContainsNode(itr->second, n1))
      {
         result.push_back(itr->second);
      }
   }
   str::sort(result);
   return result;
}

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

boost::container::static_vector<CellIndex, Topology::NumNodesOnTetrahedron>  TetrahedronsNodes::getCommonNodes(CellIndex tetId1, CellIndex tetId2) const
{
   boost::container::static_vector<CellIndex, Topology::NumNodesOnTetrahedron> result;
   const auto tetrahedronNodes1 = getTetrahedronNodes(tetId1);
   const auto tetrahedronNodes2 = getTetrahedronNodes(tetId2);
   for (auto node : tetrahedronNodes1)
   {
      if (tetrahedronNodes2.contains(node)) result.push_back(node);
   }
   return result;
}
#endif
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
#if false
std::vector<EdgeNodesSorted> TetrahedronsNodes::getAllSortedEdges() const
{
   std::vector<EdgeNodesSorted> result;
   result.reserve(3 * m_toNodes.size());
   for (auto& itr : m_toNodes)
   {
      const auto& tetrahedronNodes = itr.second;
      result.emplace_back(tetrahedronNodes.at(0), tetrahedronNodes.at(1));
      result.emplace_back(tetrahedronNodes.at(0), tetrahedronNodes.at(2));
      result.emplace_back(tetrahedronNodes.at(1), tetrahedronNodes.at(2));
   }
   str::sort(result);
   const auto [first, last] = str::unique(result);
   result.erase(first, last);
   return result;
}

std::vector<NodeIndex> TetrahedronsNodes::getAllNodes() const
{
   std::vector<NodeIndex> result;
   for (auto iter = m_toTetrahedrons.begin();
      iter != m_toTetrahedrons.end();
      iter = m_toTetrahedrons.equal_range(iter->first).second)
   {
      result.push_back(iter->first);
   }
   str::sort(result);
   return result;
}
#endif
size_t TetrahedronsNodes::getNumTetrahedrons() const
{
   return m_cellsNodes.getNumCells();
}
