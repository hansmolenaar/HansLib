#include <gtest/gtest.h>

#include "Paraview.h"

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


TEST(ParaviewTest, WriteNodes)
{
   std::ostringstream buffer;
   const std::vector<std::array<int, 2>> nodes{  { 2, 1 } };
   Paraview::WriteNodes(buffer, nodes);
   const std::string str = buffer.str();
   ASSERT_NE(str.find("POINTS"), std::string::npos);
}

TEST(ParaviewTest, Write)
{
   const std::vector<std::array<int, 2>> nodes{ {0, 0}, { 1,0 }, { 1,1 }, { 0,1 } };
   std::multimap<int, std::vector<int>> cells;
   cells.emplace(Paraview::VTK_QUAD, std::vector<int>{ 0,1, 2, 3 });
   Paraview::Write<int, 2>("HelloWorld", nodes, cells);
}
