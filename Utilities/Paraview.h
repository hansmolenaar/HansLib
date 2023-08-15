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

   template<typename T, int N>
   void Write(const std::string& baseName, const std::vector<std::array<T, N>>& points, const std::multimap<int, std::vector<int>>& cells);

   template<typename T, int N>
   void WriteNodes(std::ostream& stream, const std::vector<std::array<T, N>>& points);

   template<typename T, int N>
   void WriteNode(std::ostream& stream, const std::array<T, N>& point);

   template<typename T>
   void WriteCoordinate(std::ostream& stream, const T& cor);

   void WriteHeader(std::ostream& stream);

   void WriteCellData(std::ostream& stream, const std::multimap<int, std::vector<int>>& cells);

   void WriteCells(std::ostream& stream, const std::multimap<int, std::vector<int>>& cells);

   std::filesystem::path GetFileName(const std::string& baseName);
};

template<typename T, int N>
void Paraview::Write<T, N>(const std::string& baseName, const std::vector<std::array<T, N>>& points, const std::multimap<int, std::vector<int>>& cells)
{
   const auto path = GetFileName(baseName);
   std::ofstream stream(path.string().c_str(), std::ios::out);

   WriteHeader(stream);
   WriteNodes(stream, points);
   WriteCells(stream, cells);
   WriteCellData(stream, cells);

   stream.close();
}


template<typename T, int N>
void Paraview::WriteNodes<T, N>(std::ostream& stream, const std::vector<std::array<T, N>>& points)
{
   stream << "POINTS " << points.size() << " float\n";

   for (const auto& point : points)
   {
      WriteNode<T, N>(stream, point);
   }
   stream << "\n\n";
}

template<typename T, int N>
void Paraview::WriteNode<T, N>(std::ostream& stream, const std::array<T, N>& point)
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