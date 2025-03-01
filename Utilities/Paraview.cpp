#include "MyAssert.h"
#include "Paraview.h"

std::filesystem::path Paraview::GetFileName(const std::string& baseName)
{
   std::filesystem::path p("C:\\Users\\Hans\\Documents\\work\\");
   return p / (baseName);
}

void Paraview::WriteHeader(std::ostream& stream)
{
   stream << "# vtk DataFile Version 1.0\n";
   stream << "Hello world\n";
   stream << "ASCII\n\n";
   stream << "DATASET UNSTRUCTURED_GRID\n";
}

void Paraview::Write(const Vtk::VtkData& data)
{
   const std::string& baseName = data.getName().project;
   const auto directoryPath = GetFileName(baseName);
   std::filesystem::create_directory(directoryPath);
   const auto fileName = directoryPath / (data.getName().item + ".vtk");

   std::ofstream stream(fileName.c_str(), std::ios::out);

   WriteHeader(stream);
   WritePoints(stream, data);
   WriteCells(stream, data);
   WriteCellData(stream, data);

   stream.close();
}

void Paraview::WritePoints(std::ostream& stream, const Vtk::VtkData& data)
{
   stream << "POINTS " << data.getNumNodes() << " float\n";

   for (Vtk::NodeIndex n = 0; n < data.getNumNodes(); ++n)
   {
      const auto& point = data.getNode(n);
      WritePoint(stream, point);
   }
   stream << "\n\n";
}

void Paraview::WritePoint(std::ostream& stream, std::span<const Vtk::CoordinateType> point)
{
   for (int d = 0; d < point.size(); ++d)
   {
      stream << point[d] << " ";
   }
   for (size_t d = point.size(); d < 3; ++d)
   {
      stream << 0.0 << " ";
   }
   stream << "\n";
}

void Paraview::WriteCells(std::ostream& stream, const Vtk::VtkData& data)
{
   size_t numNodes = 0;
   for (Vtk::CellIndex c = 0; c < data.getNumCells(); ++c)
   {
      numNodes += Vtk::NumNodesForType(data.getCellType(c));
   }
   const auto numCells = data.getNumCells();
   stream << "CELLS " << numCells << " " << numCells + numNodes << "\n";

   for (Vtk::CellIndex c = 0; c < data.getNumCells(); ++c)
   {
      std::span<const Vtk::NodeIndex> nodes = data.getNodeIndices(c);
      stream << nodes.size();
      for (int n : nodes)
      {
         stream << " " << n;
      }
      stream << "\n";
   }


   stream << "\n\nCELL_TYPES " << numCells << "\n";
   for (Vtk::CellIndex c = 0; c < data.getNumCells(); ++c)
   {
      stream << static_cast<int>(data.getCellType(c)) << "\n";
   }
   stream << "\n\n";
}

void Paraview::WriteCellData(std::ostream& stream, const Vtk::VtkData& data)
{
   stream << "CELL_DATA " << data.getNumCells() << "\n\n";
   // TODO not yet implemented
   Utilities::MyAssert(data.getNumCellData() == 0);
}