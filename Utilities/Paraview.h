#pragma once

#include "VtkData.h"
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <ostream>
#include <array>
#include <fstream>

namespace Paraview
{
   void Write(const std::string& baseName, const Vtk::VtkData& data);
   void WritePoints(std::ostream& stream, const Vtk::VtkData& data);
   void WritePoint(std::ostream& stream,  std::span<const Vtk::CoordinateType> coordinates);
   void WriteCells(std::ostream& stream, const Vtk::VtkData& data);
   void WriteCellData(std::ostream& stream, const Vtk::VtkData& data);
   void WriteHeader(std::ostream& stream);

   std::filesystem::path GetFileName(const std::string& baseName);
};