#pragma once

#include "StreamUtils.h"
#include "TopologyDefines.h"

#include <optional>
#include <span>
#include <unordered_map>

namespace MeshGeneration
{
   template<typename TCell>
   class CellsNodes
   {
   public:
      CellIndex addCell(const TCell& cell);
      void deleteCell(CellIndex cellId);
      size_t getNumCells() const;

      const std::unordered_map<CellIndex, TCell>& getCellIdToNodes() const;
      const std::unordered_multimap<Topology::NodeIndex, CellIndex>& getNodeToCellIds() const;
      std::optional<CellIndex> tryGetCell(std::span<const Topology::NodeIndex> nodes) const;
      bool cellContainsNode(CellIndex CellIndex, Topology::NodeIndex nodeId) const;

      template<typename C>
      void getCommonNodes(CellIndex cellId1, CellIndex cellId2, C& containter) const;

      template<typename C>
      void getCellsContainingNodes(C& container, std::span<const Topology::NodeIndex> nodes) const;

      template<typename C>
      void getEdgeConnectedNodes(C& container, Topology::NodeIndex node) const;

      bool isKnownNodeId(Topology::NodeIndex node) const;
      bool isKnownCellId(CellIndex cellId) const;

      TCell getCellNodes(CellIndex cellId) const;

      std::vector<CellIndex> getAllCellIds() const;
      std::vector<Topology::NodeIndex> getAllNodes() const;

      void checkNodeId(Topology::NodeIndex nodeId) const;
      void checkCellId(CellIndex cellId) const;

   private:
      CellIndex m_newCellId = 0;
      std::unordered_map<CellIndex, TCell> m_cellIdToNodes;
      std::unordered_multimap<Topology::NodeIndex, CellIndex> m_nodeToCells;
   };

   template<typename TCell>
   CellIndex CellsNodes<TCell>::addCell(const TCell& cell)
   {
      // TODO add check
      // 
      // CellId must be unique to avoid an enormous mess
      const CellIndex result = m_newCellId;
      ++m_newCellId;
      m_cellIdToNodes.emplace(result, cell);
      for (auto n : cell)
      {
         m_nodeToCells.emplace(n, result);
      }
      return result;
   }

   template<typename TCell>
   size_t CellsNodes<TCell>::getNumCells() const
   {
      return m_cellIdToNodes.size();
   }

   template<typename TCell>
   const std::unordered_map<CellIndex, TCell>& CellsNodes<TCell>::getCellIdToNodes() const
   {
      return m_cellIdToNodes;
   };

   template<typename TCell>
   const std::unordered_multimap<Topology::NodeIndex, CellIndex>& CellsNodes<TCell>::getNodeToCellIds() const
   {
      return m_nodeToCells;
   };

   template<typename TCell>
   void CellsNodes<TCell>::checkNodeId(Topology::NodeIndex nodeId) const
   {
      if (!isKnownNodeId(nodeId))
      {
         const std::string msg = "CellsNodes<TCell>::checkNodeId() unknown NodeId " + std::to_string(nodeId);
         throw MyException(msg);
      }
   }

   template<typename TCell>
   void CellsNodes<TCell>::checkCellId(CellIndex cellId) const
   {
      if (!isKnownCellId(cellId))
      {
         const std::string msg = "CellsNodes<TCell>::checkCellId() unknown cellId " + std::to_string(cellId);
         throw MyException(msg);
      }
   }

   template<typename TCell>
   bool CellsNodes<TCell>::isKnownNodeId(Topology::NodeIndex node) const
   {
      return m_nodeToCells.find(node) != m_nodeToCells.end();
   }

   template<typename TCell>
   bool CellsNodes<TCell>::isKnownCellId(CellIndex cellId) const
   {
      return m_cellIdToNodes.find(cellId) != m_cellIdToNodes.end();
   }

   template<typename TCell>
   std::optional<CellIndex> CellsNodes<TCell>::tryGetCell(std::span<const Topology::NodeIndex> nodes) const
   {
      for (auto n : nodes)
      {
         checkNodeId(n);
      }

      const auto cellRange = m_nodeToCells.equal_range(nodes[0]);
      for (auto itr = cellRange.first; itr != cellRange.second; ++itr)
      {
         const CellIndex candidate = itr->second;
         const auto& candidateNodes = m_cellIdToNodes.at(candidate);
         if (str::equal(nodes, candidateNodes))
         {
            // Duplicates are not possible, already checked in addCell
            return candidate;
         }
      }
      return {};
   }

   template<typename TCell>
   bool CellsNodes<TCell>::cellContainsNode(CellIndex cellId, Topology::NodeIndex nodeId) const
   {
      checkCellId(cellId); // only for error message
      checkNodeId(nodeId);
      const auto& nodes = m_cellIdToNodes.at(cellId);
      return str::find(nodes, nodeId) != nodes.end();
   }


   template<typename TCell>
   TCell CellsNodes<TCell>::getCellNodes(CellIndex cellId) const
   {
      checkCellId(cellId); // only for error message
      return m_cellIdToNodes.at(cellId);
   }

   template<typename TCell>
   void CellsNodes<TCell>::deleteCell(CellIndex cellId)
   {
      checkCellId(cellId);
      const auto& nodes = m_cellIdToNodes.at(cellId);
      for (auto node : nodes)
      {
         const auto [first, last] = m_nodeToCells.equal_range(node);
         bool found = false;
         for (auto itr = first; itr != last; ++itr)
         {
            if (itr->second == cellId)
            {
               found = true;
               m_nodeToCells.erase(itr);
               break;
            }
         }
         if (!found)
         {
            const std::string msg = " CellsNodes<TCell>::::deleteCell() inconsistency in m_nodeToCells: " + std::to_string(cellId) + " " + std::to_string(node);
            throw MyException(msg);
         }
      }
      m_cellIdToNodes.erase(cellId);
   }

   template<typename TCell>
   std::vector<CellIndex> CellsNodes<TCell>::getAllCellIds() const
   {
      std::vector<CellIndex> result;
      result.reserve(m_cellIdToNodes.size());
      for (auto& itr : m_cellIdToNodes)
      {
         result.push_back(itr.first);
      }
      str::sort(result);
      return result;
   }
   template<typename TCell>
   std::vector<Topology::NodeIndex> CellsNodes<TCell>::getAllNodes() const
   {
      std::vector<Topology::NodeIndex> result;
      for (auto iter = m_nodeToCells.begin();
         iter != m_nodeToCells.end();
         iter = m_nodeToCells.equal_range(iter->first).second)
      {
         result.push_back(iter->first);
      }
      str::sort(result);
      return result;
   }

   template<typename TCell>
   template<typename C>
   void CellsNodes<TCell>::getCommonNodes(CellIndex cellId1, CellIndex cellId2, C& result) const
   {
      checkCellId(cellId1);
      checkCellId(cellId2);

      result.clear();
      const auto& cellNodes1 = m_cellIdToNodes.at(cellId1);
      const auto& cellNodes2 = m_cellIdToNodes.at(cellId2);
      for (auto node : cellNodes1)
      {
         if (cellNodes2.contains(node)) result.push_back(node);
      }
   }

   template<typename TCell>
   template<typename C>
   void CellsNodes<TCell>::getCellsContainingNodes(C& result, std::span<const Topology::NodeIndex> nodes) const
   {
      result.clear();

      const auto node0 = nodes[0];
      checkNodeId(node0);
      const auto [first, last] = getNodeToCellIds().equal_range(node0);
      for (auto itr = first; itr != last; ++itr)
      {
         const auto cellId = itr->second;
         if (std::all_of(nodes.begin() + 1, nodes.end(), [this, cellId](auto nodeId) {return cellContainsNode(cellId, nodeId); }))
         {
            result.push_back(cellId);
         }
      }
      str::sort(result);
   }

   template<typename TCell>
   template<typename C>
   void CellsNodes<TCell>::getEdgeConnectedNodes(C& result, Topology::NodeIndex node) const
   {
      result.clear();

      checkNodeId(node);
      const auto [first, last] = m_nodeToCells.equal_range(node);
      for (auto itr = first; itr != last; ++itr)
      {
         const auto cellId = itr->second;
         const auto& cellNodes = getCellNodes(cellId);
         for (auto ngb : cellNodes)
         {
            if ((ngb != node) && (str::find(result, ngb) == result.end()))
            {
               result.push_back(ngb);
            }
         }
      }
      str::sort(result);
   }
}