#pragma once

#include "VtkData.h"
#include <filesystem>
#include <ostream>
#include <string>
#include <vector>

namespace Paraview
{
void WriteList(const std::vector<const Vtk::VtkData *> &data);
void Write(const Vtk::VtkData &data);
void WritePoints(std::ostream &stream, const Vtk::VtkData &data);
void WritePoint(std::ostream &stream, std::span<const Vtk::CoordinateType> coordinates);
void WriteCells(std::ostream &stream, const Vtk::VtkData &data);
void WriteCellData(std::ostream &stream, const Vtk::VtkData &data);
void WriteHeader(std::ostream &stream);

std::filesystem::path GetFileName(const std::string &baseName);
}; // namespace Paraview
