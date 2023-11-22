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

std::unordered_multimap<int, FieldIndex> SolverSweepBiValueLinks::GetColoredNodes(const Potentials& potentials, Value value)
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

   Graph g;

   std::unordered_map<FieldIndex, boost::adjacency_list<>::vertex_descriptor> field2vd;
   for (auto field : activeFields)
   {
      boost::adjacency_list<>::vertex_descriptor vd = add_vertex(g);
      g[vd] = { field };
      field2vd.emplace(field, vd);
   }

   // Set edges
   for (const auto a : adjacencies)
   {
      const auto v1 = field2vd.at(a.first);
      const auto v2 = field2vd.at(a.second);
      add_edge(v1, v2, g);
   }

   return {};
}