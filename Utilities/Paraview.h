#pragma once

#include "VtkData.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace Paraview
{
   void WriteList(const std::vector<const Vtk::VtkData*>& data);
   void WriteList(const std::vector<std::unique_ptr<Vtk::VtkData>>& data); // TODO remove me
   void Write(const Vtk::VtkData& data);
   void WritePoints(std::ostream& stream, const Vtk::VtkData& data);
   void WritePoint(std::ostream& stream, std::span<const Vtk::CoordinateType> coordinates);
   void WriteCells(std::ostream& stream, const Vtk::VtkData& data);
   void WriteCellData(std::ostream& stream, const Vtk::VtkData& data);
   void WriteHeader(std::ostream& stream);

   std::filesystem::path GetFileName(const std::string& baseName);
};