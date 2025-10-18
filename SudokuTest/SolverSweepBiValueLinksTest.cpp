#include <gtest/gtest.h>

#include "SolverSweepBiValueLinks.h"
#include "TestModels.h"
#include "Defines.h"
#include "FieldInfoStatic.h"
#include "GraphDefines.h"

using namespace Sudoku;
using namespace boost;

TEST(SolverSweepBiValueLinksTest, MyFirstBoostGraph)
{
   struct Vertex { int vertexId; int color; };
   struct Edge { };
   using  SudokuGraph = Graph<Vertex, Edge>;

   constexpr int numVertices = 4;
   SudokuGraph g;

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
   using  SudokuGraph = Graph<Vertex, Edge>;

   constexpr int numVertices = 5;
   SudokuGraph g;

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
   ASSERT_NE(colorsCmp.front().CurrentColor, colorsCmp.back().CurrentColor);
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
