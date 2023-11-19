#include <gtest/gtest.h>

#include "SolverSweep.h"
#include "FieldInfoStatic.h"
#include "Diagram.h"
#include "TestModels.h"

#include <unordered_map>
#include "boost/graph/adjacency_list.hpp"
#include <boost/graph/connected_components.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>
#include <iostream>

using namespace Sudoku;
using namespace boost;

TEST(SolverSweepTest, MyFirstBoostGraph)
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


TEST(SolverSweepTest, SolverSweepAll)
{
   SolverSweep sweep;
   Diagram diagramEmpty = TestModels::getEmpty();
   auto potentials = diagramEmpty.getPotentials();
   auto result = sweep(potentials);
   ASSERT_EQ(result, SolverSweepResult::NoChange);
}


TEST(SolverSweepTest, SolverSweepSolved)
{
   SolverSweep sweep;
   Diagram diagramSolved = TestModels::getSolved();
   auto potentials = diagramSolved.getPotentials();
   auto result = sweep(potentials);
   ASSERT_EQ(result, SolverSweepResult::Solved);
}
