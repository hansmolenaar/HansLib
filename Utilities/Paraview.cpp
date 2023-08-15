#include "Paraview.h"


std::filesystem::path Paraview::GetFileName(const std::string& baseName)
{
   std::filesystem::path p("C:\\Users\\Hans\\Documents\\work\\");
   return p / (baseName + ".vtk");
}

void Paraview::WriteHeader(std::ostream& stream)
{
   stream << "# vtk DataFile Version 1.0\n";
   stream << "Hello world\n";
   stream << "ASCII\n\n";
   stream << "DATASET UNSTRUCTURED_GRID\n";
}

void Paraview::WriteCells(std::ostream& stream, const std::multimap<int, std::vector<int>>& cells)
{
   int numNodes = 0;
   for (const auto& itr : cells)
   {
      numNodes = static_cast<int>(itr.second.size());
   }
   const auto numCells = cells.size();
   stream << "CELLS " << numCells << " " << numCells + numNodes << "\n";

   for (const auto& itr : cells)
   {
      stream << itr.second.size();
      for (int n : itr.second)
      {
         stream << " " << n;
      }
      stream << "\n";
   }


   stream << "\n\nCELL_TYPES " << numCells << "\n";
   for (const auto& itr : cells)
   {
      stream << itr.first << "\n";
   }
   stream << "\n\n";
}

void Paraview::WriteCellData(std::ostream& stream, const std::multimap<int, std::vector<int>>& cells)
{
   stream << "CELL_DATA " << cells.size() << "\n\n";
}