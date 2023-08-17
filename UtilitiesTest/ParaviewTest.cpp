#include <gtest/gtest.h>

#include "Paraview.h"
#include "VtkData.h"

using namespace Vtk;

TEST(ParaviewTest, GetFileName)
{
   std::filesystem::path path = Paraview::GetFileName("foo");
   const auto str = path.generic_string();
   ASSERT_NE(str.find("foo"), std::string::npos);
}


TEST(ParaviewTest, WriteHeader)
{
   std::ostringstream buffer;
   Paraview::WriteHeader(buffer);
   const std::string str = buffer.str();
   ASSERT_NE(str.find("UNSTRUCTURED_GRID"), std::string::npos);
}


TEST(ParaviewTest, WritePoints)
{
   VtkData data(2, 0);
   const std::vector<std::array<CoordinateType, 2>> nodes{ { 2, 1 } };
   data.addNode(nodes.front());

   std::ostringstream buffer;
   Paraview::WritePoints(buffer, data);
   const std::string str = buffer.str();
   ASSERT_NE(str.find("POINTS"), std::string::npos);
}

TEST(ParaviewTest, Write)
{
   const std::vector<std::array<CoordinateType, 2>> nodes{ {0, 0}, { 1,0 }, { 1,1 }, { 0,1 } };
   VtkData data(2, 0);
   for (const auto& node : nodes)
   {
      data.addNode(node);
   }

   const std::vector<NodeIndex> nodeIndices{ 0, 1, 2, 3 };
   data.addCell(CellType::VTK_QUAD, nodeIndices, {});

   Paraview::Write("HelloWorld", data);
}
