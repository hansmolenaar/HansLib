#include "SolverSweepBiValueLinks.h"
#include "FieldInfoStatic.h"
#include "Defines.h"

#include <iterator>
#include <unordered_set>
#include "boost/graph/adjacency_list.hpp"
#include <boost/graph/connected_components.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>

using namespace Sudoku;


SolverSweepResult SolverSweepBiValueLinks::operator()(Potentials& potentials)
{
   for (const FieldIndex field : FieldInfoStatic::getAllFields())
   {

   }
   return SolverSweepResult::NoChange;
}

ActiveFields SolverSweepBiValueLinks::GetBiValueFields(const Potentials& potentials, Value value)
{
   ActiveFields result;
   str::copy_if(FieldInfoStatic::getAllFields(), std::back_inserter(result),
      [&potentials, value](FieldIndex f) {return potentials.get(f).count() == 2 && potentials.get(f).containsValue(value); });
   return result;
}

std::vector<std::pair<FieldIndex, FieldIndex>> SolverSweepBiValueLinks::GetBiValueAdjecencies(const Potentials& potentials, Value value)
{
   ActiveFields active = GetBiValueFields(potentials, value);
   if (active.empty()) return {};

   // Try all combinations
   std::vector<std::pair<FieldIndex, FieldIndex>> result;
   str::sort(active);
   for (size_t n1 = 0; n1 < active.size(); ++n1)
   {
      for (size_t n2 = n1 + 1; n2 < active.size(); ++n2)
      {
         const auto f1 = active.at(n1);
         const auto f2 = active.at(n2);
         if (FieldInfoStatic::AreConnected(f1, f2))
         {
            result.emplace_back(std::make_pair(f1, f2));
         }
      }
   }
   return result;
}

ColorInAllComponents SolverSweepBiValueLinks::GetColoring(const Potentials& potentials, Value value)
{
   const auto adjacencies = GetBiValueAdjecencies(potentials, value);
   if (adjacencies.empty()) return {};

   // Get the active fields
   std::unordered_set<FieldIndex> activeFields;
   for (const auto a : adjacencies)
   {
      activeFields.insert(a.first);
      activeFields.insert(a.second);
   }

   struct Vertex { FieldIndex Field; };
   struct Edge { };
   typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, Vertex, Edge > Graph;


   using VertexDesc = boost::adjacency_list<>::vertex_descriptor;
   Graph g;
   std::unordered_map<FieldIndex, VertexDesc> field2vd;
   for (auto field : activeFields)
   {
      const VertexDesc vd = add_vertex(g);
      g[vd] = { field };
      field2vd.emplace(field, vd);
   }
   const auto numVertices = boost::num_vertices(g);

   // Set edges
   for (const auto a : adjacencies)
   {
      const auto v1 = field2vd.at(a.first);
      const auto v2 = field2vd.at(a.second);
      add_edge(v1, v2, g);
   }

   // Components
   std::vector<int> components(numVertices);
   size_t num_components = boost::connected_components(g, &components[0]);

   // Colors
   std::vector<size_t> color_vec(numVertices);
   auto index_map = get(boost::vertex_index, g);
   auto color_map = make_safe_iterator_property_map(
      color_vec.begin(), color_vec.size(), index_map);
   auto num_colors = sequential_vertex_coloring(g, color_map);

   ColorInAllComponents result(num_components);
   for (VertexDesc v = 0; v < numVertices; ++v)
   {
      const auto component = components.at(v);
      const auto color = static_cast<Color>(color_vec.at(v));
      const auto field = g[v].Field;
      result[component].push_back(FieldColor{field, color});
   }
   return result;
}