#include <gtest/gtest.h>

#include "SolverSweepBiValueLinks.h"
#include "TestModels.h"
#include "Defines.h"
#include "FieldInfoStatic.h"
#include "boost/graph/adjacency_list.hpp"
#include <boost/graph/connected_components.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>

using namespace Sudoku;
using namespace boost;

TEST(SolverSweepBiValueLinksTest, MyFirstBoostGraph)
{
   struct Vertex { int vertexId; int color; };
   struct Edge { };
   typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, Vertex, Edge > Graph;

   constexpr int numVertices = 4;
   Graph g;

   for (int v = 0; v < numVertices; ++v)
   {
      adjacency_list<>::vertex_descriptor vd = add_vertex(g);
      g[vd] = { 2 * v, 999 };
   }

   // Looping over the vertices
   const std::vector<int> expect{ 0, 2, 4, 6 };
   std::vector<int> actual;
   for (const auto v : boost::make_iterator_range(vertices(g)))
   {
      const auto& data = g[v];
      actual.push_back(g[v].vertexId);
   }

   ASSERT_TRUE(expect == actual);

   // Connected components
   add_edge(1, 2, g);
   std::vector<int> component(boost::num_vertices(g));
   size_t num_components = boost::connected_components(g, &component[0]);
   ASSERT_EQ(num_components, 3);

   // Coloring
   add_edge(3, 2, g);
   add_edge(0, 1, g);
   add_edge(0, 3, g);

   std::fill(component.begin(), component.end(), 0);
   num_components = boost::connected_components(g, &component[0]);
   ASSERT_EQ(num_components, 1);

   std::vector<size_t> color_vec(num_vertices(g));
   auto index_map = get(boost::vertex_index, g);

   auto color_map = make_safe_iterator_property_map(
      color_vec.begin(), color_vec.size(), index_map);

   auto num_colors = sequential_vertex_coloring(g, color_map);
   ASSERT_EQ(num_colors, 2);

   const std::vector<size_t> expectColors{ 0,1,0,1 };
   ASSERT_TRUE(color_vec == expectColors);
}

TEST(SolverSweepBiValueLinksTest, ColoringAndMultipleComponents)
{
   struct Vertex { int vertexId; };
   struct Edge { };
   typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, Vertex, Edge > Graph;

   constexpr int numVertices = 5;
   Graph g;

   for (int v = 0; v < numVertices; ++v)
   {
      adjacency_list<>::vertex_descriptor vd = add_vertex(g);
      g[vd] = { v };
   }

   // Coloring of disconnected components
   add_edge(1, 2, g);
   add_edge(0, 3, g);

   std::vector<size_t> color_vec(num_vertices(g));
   auto index_map = get(boost::vertex_index, g);

   auto color_map = make_safe_iterator_property_map(
      color_vec.begin(), color_vec.size(), index_map);

   auto num_colors = sequential_vertex_coloring(g, color_map);
   ASSERT_EQ(num_colors, 2);

   const std::vector<size_t> expectColors{ 0, 0, 1, 1, 0 };
   ASSERT_TRUE(color_vec == expectColors);
}

TEST(SolverSweepBiValueLinksTest, BiValueFieldGraphColored)
{
   Potentials potentials;

   potentials.setForTesting(FieldInfoStatic::RowColToField(1, 2), { 4,5 });
   potentials.setForTesting(FieldInfoStatic::RowColToField(2, 2), { 5,6 });

   const ColorInAllComponents colors = SolverSweepBiValueLinksSingleValue::GetColoring(potentials, 5);
   ASSERT_EQ(colors.size(), 1);
   const ColorInComponent& colorsCmp = colors.at(0);
   ASSERT_EQ(colorsCmp.size(), 2);
   ASSERT_NE(colorsCmp.front().Color, colorsCmp.back().Color);
}

TEST(SolverSweepBiValueLinksTest, GetBiValueFields)
{
   const Diagram diagram = TestModels::getBiValue1();
   auto potentials = diagram.getPotentials();
   SolverSweepTrivial sweep;
   const auto status = sweep(potentials);
   const auto start = potentials.toString();
   ASSERT_EQ(potentials.getNumSingles(), 53);

   const auto biValueFields = SolverSweepBiValueLinksSingleValue::GetBiValueFields(potentials, 4);
   ASSERT_EQ(biValueFields.size(), 9);
}


TEST(SolverSweepBiValueLinksTest, GetBiValueAdjecencies)
{
   const Diagram diagram = TestModels::getBiValue1();
   auto potentials = diagram.getPotentials();
   SolverSweepTrivial sweep;
   sweep(potentials);

   const auto adjacencies = SolverSweepBiValueLinksSingleValue::GetBiValueAdjecencies(potentials, 7);
   ASSERT_EQ(adjacencies.size(), 3);
   ASSERT_EQ(2, str::count_if(adjacencies, [](const auto p) {return p.first == 8 || p.second == 8; }));
}


TEST(SolverSweepBiValueLinksTest, BiValueFieldSweep)
{
   Potentials potentials;
   constexpr FieldIndex field20 = FieldInfoStatic::RowColToField(2, 0);
   constexpr FieldIndex field50 = FieldInfoStatic::RowColToField(5, 0);
   constexpr FieldIndex field32 = FieldInfoStatic::RowColToField(3, 2);
   constexpr FieldIndex field62 = FieldInfoStatic::RowColToField(6, 2);
   constexpr FieldIndex field64 = FieldInfoStatic::RowColToField(6, 4);
   constexpr FieldIndex field24 = FieldInfoStatic::RowColToField(2, 4);
   potentials.setForTesting(field20, { 5,1 });
   potentials.setForTesting(field50, { 5,2 });
   potentials.setForTesting(field32, { 5,3 });
   potentials.setForTesting(field62, { 5,4 });
   potentials.setForTesting(field64, { 5,6 });
   potentials.setForTesting(field24, { 7,5 });

   const auto countBefore = potentials.getTotalCount();

   constexpr Value commonValue = 5;
   SolverSweepBiValueLinksSingleValue sweep(commonValue);
   const auto status = sweep(potentials);
   ASSERT_EQ(status, SolverSweepResult::NoChange);

   const auto countAfter = potentials.getTotalCount();
   ASSERT_EQ(countAfter + 2, countBefore);
}