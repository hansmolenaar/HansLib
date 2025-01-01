#pragma once

#include <array>
#include <filesystem>
#include <fstream>
#include <map>
#include <ostream>
#include <span>
#include <string>
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
   };

}
