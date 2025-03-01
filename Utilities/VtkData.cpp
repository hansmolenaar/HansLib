#include "MyAssert.h"
#include "MyException.h"
#include "VtkData.h"

using namespace Vtk;
using namespace Utilities;

constexpr size_t Vtk::NumNodesForType(CellType cellType)
{
   switch (cellType)
   {
   case CellType::VTK_LINE: return 2;
   case CellType::VTK_TRIANGLE: return 3;
   case CellType::VTK_QUAD: return 4;
   case CellType::VTK_TETRA: return 4;
   case CellType::VTK_HEXAHEDRON: return 8;

   default:
      throw MyException("NumNodesForType: unknown type");
   }
}

VtkData::VtkData(int geomDim, int numData) :
   m_geomDim(geomDim),
   m_numData(numData)
{
}

void VtkData::addNode(std::span<const CoordinateType> coordinates)
{
   MyAssert(coordinates.size() == m_geomDim);
   m_coordinates.insert(m_coordinates.end(), coordinates.begin(), coordinates.end());
}


void VtkData::addCell(CellType typ, std::span<const NodeIndex> nodes, std::span<const DataType>data)
{
   MyAssert(nodes.size() == NumNodesForType(typ));
   MyAssert(data.size() == m_numData);

   m_type.push_back(typ);
   m_nodeOfset.push_back(m_nodeIndices.size());
   m_nodeIndices.insert(m_nodeIndices.end(), nodes.begin(), nodes.end());
   m_data.insert(m_data.end(), data.begin(), data.end());
}

NodeIndex VtkData::getNumNodes() const
{
   return static_cast<NodeIndex>(m_coordinates.size() / m_geomDim);
}

std::span<const CoordinateType> VtkData::getNode(NodeIndex n) const
{
   const auto& first = m_coordinates.begin() + n * m_geomDim;
   return std::span<const CoordinateType>(first, m_geomDim);

}

Vtk::CellIndex VtkData::getNumCells() const
{
   return static_cast<CellIndex>(m_type.size());
}

CellType VtkData::getCellType(CellIndex n) const
{
   return m_type.at(n);
}

std::span<const NodeIndex> VtkData::getNodeIndices(CellIndex n) const
{
   return  { m_nodeIndices.begin() + m_nodeOfset.at(n),NumNodesForType(getCellType(n)) };
}

std::span<const DataType> VtkData::getCellData(CellIndex n) const
{
   return { m_data.begin() + n * m_numData, m_numData };
}

size_t VtkData::getNumCellData() const
{
   return m_numData;
}
