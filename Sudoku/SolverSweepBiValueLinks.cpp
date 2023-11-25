#include "SolverSweepBiValueLinks.h"
#include "FieldInfoStatic.h"
#include "Defines.h"
#include "MyAssert.h"

#include <iterator>
#include <unordered_set>
#include "boost/graph/adjacency_list.hpp"
#include <boost/graph/connected_components.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>

using namespace Sudoku;

SolverSweepBiValueLinksSingleValue::SolverSweepBiValueLinksSingleValue(Value value) :
   m_value(value)
{
}


ActiveFields SolverSweepBiValueLinksSingleValue::GetBiValueFields(const Potentials& potentials, Value value)
{
   ActiveFields result;
   str::copy_if(FieldInfoStatic::getAllFields(), std::back_inserter(result),
      [&potentials, value](FieldIndex f) {return potentials.get(f).count() == 2 && potentials.get(f).containsValue(value); });
   return result;
}

std::vector<std::pair<FieldIndex, FieldIndex>> SolverSweepBiValueLinksSingleValue::GetBiValueAdjecencies(const Potentials& potentials, Value value)
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

ColorInAllComponents SolverSweepBiValueLinksSingleValue::GetColoring(const Potentials& potentials, Value value)
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
      result[component].push_back(FieldColor{ field, color });
   }
   return result;
}

Value SolverSweepBiValueLinksSingleValue::getOtherValue(const Potentials& potentials, FieldIndex field) const
{
   const PotentialValues potentialPair = potentials.get(field).getPotentialValues();
   Utilities::MyAssert(potentialPair.size() == 2);
   if (potentialPair.front() == m_value) return potentialPair.back();
   if (potentialPair.back() == m_value) return potentialPair.front();
   throw MyException("SolverSweepBiValueLinksSingleValue::getOtherValue() should not come here");
}

SolverSweepResult SolverSweepBiValueLinksSingleValue::operator()(Potentials& potentials)
{
   SolverSweepResult result = SolverSweepResult::NoChange;
   const ColorInAllComponents coloring = GetColoring(potentials, m_value);
   for (const auto& coloredComponent : coloring)
   {
      for (size_t n1 = 0; n1 < coloredComponent.size(); ++n1)
      {
         for (size_t n2 = n1 + 1; n2 < coloredComponent.size(); ++n2)
         {
            const bool sameColor = coloredComponent.at(n1).Color == coloredComponent.at(n2).Color;
            if (!sameColor) continue;
            const FieldIndex field1 = coloredComponent.at(n1).Field;
            const FieldIndex field2 = coloredComponent.at(n2).Field;
            if (FieldInfoStatic::AreConnected(field1, field2))
            {
               if (potentials.unset(field1, m_value))
               {
                  result = std::max(result, SolverSweepResult::Change);
               }
               if (potentials.unset(field2, m_value))
               {
                  result = std::max(result, SolverSweepResult::Change);
               }
            }
         }
      }
   }

   if (potentials.isSolved()) return SolverSweepResult::Solved;
   return result;
}