#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <ostream>
#include <array>
#include <fstream>

namespace Paraview
{
   constexpr int VTK_QUAD = 9;

   struct CellData
   {
      int CellType;
      std::vector<int> Nodes;
      std::vector<double> Data;
   };

   template<typename T, int N>
   void Write(const std::string& baseName, const std::vector<std::array<T, N>>& points, const std::vector<CellData>& cells);

   template<typename T, int N>
   void WritePoints(std::ostream& stream, const std::vector<std::array<T, N>>& points);

   template<typename T, int N>
   void WritePoint(std::ostream& stream, const std::array<T, N>& point);

   template<typename T>
   void WriteCoordinate(std::ostream& stream, const T& cor);

   void WriteHeader(std::ostream& stream);

   void WriteCellData(std::ostream& stream, const std::vector<CellData>& cells);

   void WriteCells(std::ostream& stream, const std::vector<CellData>& cells);

   std::filesystem::path GetFileName(const std::string& baseName);
};

template<typename T, int N>
void Paraview::Write<T, N>(const std::string& baseName, const std::vector<std::array<T, N>>& points, const std::vector<CellData>& cells)
{
   const auto path = GetFileName(baseName);
   std::ofstream stream(path.string().c_str(), std::ios::out);

   WriteHeader(stream);
   WritePoints(stream, points);
   WriteCells(stream, cells);
   WriteCellData(stream, cells);

   stream.close();
}


template<typename T, int N>
void Paraview::WritePoints<T, N>(std::ostream& stream, const std::vector<std::array<T, N>>& points)
{
   stream << "POINTS " << points.size() << " float\n";

   for (const auto& point : points)
   {
      WritePoint<T, N>(stream, point);
   }
   stream << "\n\n";
}

template<typename T, int N>
void Paraview::WritePoint<T, N>(std::ostream& stream, const std::array<T, N>& point)
{
   for (int d = 0; d < N; ++d)
   {
      stream << static_cast<float>(point[d]) << " ";
   }
   for (int d = N; d < 3; ++d)
   {
      stream << 0.0 << " ";
   }
   stream << "\n";
}

template<typename T>
void Paraview::WriteCoordinate(std::ostream& stream, const T& cor)
{
   stream << static_cast<float>(cor) << " ";
}