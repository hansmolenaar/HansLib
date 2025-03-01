#pragma once

#include "IPointCollection.h"

#include <array>
#include <filesystem>
#include <fstream>
#include <map>
#include <ostream>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

namespace Vtk
{
   using DataType = float;
   using CoordinateType = float;
   using NodeIndex = int;
   using CellIndex = int;

   enum class CellType : int { VTK_LINE = 3, VTK_TRIANGLE = 5, VTK_QUAD = 9, VTK_TETRA = 10, VTK_HEXAHEDRON = 12 };
   constexpr size_t NumNodesForType(CellType cellType);

   class VtkData
   {
   public:
      VtkData(int geomDim, int numData);

      void addNode(std::span<const CoordinateType> coordinates);
      void addCell(CellType typ, std::span<const NodeIndex> nodes, std::span<const DataType>data);

      CellIndex getNumCells() const;
      NodeIndex getNumNodes() const;
      std::span<const CoordinateType> getNode(NodeIndex n) const;
      CellType getCellType(CellIndex n) const;
      std::span<const NodeIndex> getNodeIndices(CellIndex n) const;
      std::span<const DataType> getCellData(CellIndex n) const;
      size_t getNumCellData() const;

      template<typename T, int N>
      void addCell(CellType typ, std::span<const PointIndex> points, const IPointCollection<T, N>& pointCollection, std::span<const DataType>data);

   private:
      size_t m_geomDim;
      size_t m_numData;

      // Node
      std::vector<CoordinateType> m_coordinates;

      // Cell
      std::vector<CellType> m_type;
      std::vector<NodeIndex> m_nodeIndices;
      std::vector<size_t> m_nodeOfset; // offset in m_nodeIndices
      std::vector<DataType> m_data;
      std::unordered_map<PointIndex, NodeIndex> m_pointToNodeIndex;
   };

   template<typename T, int N>
   void VtkData::addCell(CellType typ, std::span<const PointIndex> points, const IPointCollection<T, N>& pointCollection, std::span<const DataType>data)
   {
      std::vector<NodeIndex> nodeIndices;
      for (auto p : points)
      {
         auto found = m_pointToNodeIndex.find(p);
         if (!m_pointToNodeIndex.contains(p))
         {
            const auto& pointInCollection = pointCollection.getPoint(p);
            std::array<CoordinateType, N> coordinates;
            // Handle rational
            str::transform(pointInCollection, coordinates.begin(), [](T c) {return static_cast<CoordinateType>(1.0 * c); });
            addNode(coordinates);

            m_pointToNodeIndex[p] = static_cast<NodeIndex>(m_pointToNodeIndex.size());
            found = m_pointToNodeIndex.find(p);
         }
         nodeIndices.push_back(found->second);
      }
      addCell(typ, nodeIndices, data);
   }
}
