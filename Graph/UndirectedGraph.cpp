#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Utilities;

namespace
{
   constexpr GraphVertex DegreeSequenceDone = std::numeric_limits<GraphVertex>::max();

   // If a polygon is found the last node of the resulting list equals the first one
   std::vector<GraphVertex> TraceLineOrPolygon(const UndirectedGraph& graph, GraphVertex start, GraphVertex nxt, std::vector<GraphVertex>& degreeSequence)
   {
      std::vector<GraphVertex> result;
      std::vector<GraphVertex>  ngbVertices;
      result.push_back(start);
      while (true)
      {
         result.push_back(nxt);
         if (nxt == start)
         {
            // interesting, a loop
            MyAssert(result.size() > 3); // not a proper labeled graph: loop
            return result;
         }
         else if (degreeSequence[nxt] == 2)
         {
            degreeSequence[nxt] = DegreeSequenceDone; // Mark as done
            // Find next vertex
            graph.setAdjacentVertices(nxt, ngbVertices);
            MyAssert(ngbVertices.size() == 2);
            nxt = (ngbVertices[0] == result[result.size() - 2] ? ngbVertices[1] : ngbVertices[0]);
         }
         else
         {
            if (degreeSequence.at(nxt) == 1)
            {
               degreeSequence.at(nxt) = DegreeSequenceDone;
            }
            // Last point of line reached
            return result;
         }
      }
   }
}


UndirectedGraph::UndirectedGraph(GraphVertex numVertices) : m_graph(numVertices)
{
}

void UndirectedGraph::addEdge(GraphVertex vertex1, GraphVertex vertex2)
{
   if (!areAdjacent(vertex1, vertex2))
   {
      boost::add_edge(vertex1, vertex2, m_graph);
   }
}

GraphVertex UndirectedGraph::getNumVertices() const
{
   return boost::num_vertices(m_graph);
}

std::vector<GraphVertex> UndirectedGraph::getConnectedComponents() const
{
   std::vector<GraphVertex> result(getNumVertices());
   boost::connected_components(m_graph, result.data());
   return result;
}

void UndirectedGraph::setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex>& result) const
{
   result.clear();
   const auto neighbours = boost::adjacent_vertices(vertex, m_graph);
   for (auto vd : make_iterator_range(neighbours))
   {
      result.push_back(vd);
   }
   str::sort(result);
}

GraphEdge UndirectedGraph::getNumEdges() const
{
   return boost::num_edges(m_graph);
}

GraphVertex UndirectedGraph::getDegree(GraphVertex vertex) const
{
   const auto neighbours = boost::adjacent_vertices(vertex, m_graph);
   return std::distance(neighbours.first, neighbours.second);
}

std::vector<GraphVertex> UndirectedGraph::getDegreeSequence() const
{
   const auto numVertices = getNumVertices();
   std::vector<GraphVertex> result(numVertices);
   for (GraphVertex v = 0; v < numVertices; ++v)
   {
      result[v] = getDegree(v);
   }
   return result;
}

// Isolated vertices are ignored
UndirectedGraph::CyclesAndPaths UndirectedGraph::SplitInCyclesAndPaths() const
{
   CyclesAndPaths result;

   const auto vertexCount = getNumVertices();
   auto degreeSequence = getDegreeSequence();
   std::vector<GraphVertex> ngbVertices;

   for (GraphVertex v = 0; v < vertexCount; ++v)
   {
      // Set degree sequence to -1 if done
      if (degreeSequence.at(v) != DegreeSequenceDone && degreeSequence.at(v) != 2)
      {
         // New lines starts here, loop over the edges that contain this node
         //neighbors.
         setAdjacentVertices(v, ngbVertices);
         for (auto ngb : ngbVertices)
         {
            if (degreeSequence.at(ngb) != DegreeSequenceDone)
            {
               std::vector<GraphVertex> list = TraceLineOrPolygon(*this, v, ngb, degreeSequence);
               if (list.front() != list.back())
               {
                  result.Paths.emplace_back(std::move(list));
               }
               else
               {
                  list.pop_back();
                  result.Cycles.emplace_back(std::move(list));
               }
            }
         }
         degreeSequence.at(v) = DegreeSequenceDone; // Done here
      }
   }

   // But there is more: isolated loops
   for (GraphVertex v = 0; v < vertexCount; ++v)
   {
      if (degreeSequence.at(v) == 2)
      {
         setAdjacentVertices(v, ngbVertices);
         MyAssert(ngbVertices.size() == 2);
         auto list = TraceLineOrPolygon(*this, v, ngbVertices.front(), degreeSequence);
         MyAssert(list.front() == list.back());
         list.pop_back(); // remove last
         result.Cycles.emplace_back(std::move(list));
         degreeSequence.at(v) = DegreeSequenceDone; // Done here
      }
   }

   MyAssert(str::all_of(degreeSequence, [](GraphVertex v) {return v == DegreeSequenceDone; }));
   return result;
}

bool UndirectedGraph::areAdjacent(GraphVertex v1, GraphVertex v2) const
{
   if (std::max(v1, v2) >= getNumVertices())
   {
      throw MyException("UndirectedGraph::areAdjacent invalid vertex");
   }

   auto [itr, found] = edge(vertex(v1, m_graph), vertex(v2, m_graph), m_graph);
   return found;
}

std::vector<GraphVertex> UndirectedGraph::getIsolatedVertices() const
{
   std::vector<GraphVertex> result;
   for (GraphVertex v = 0; v < getNumVertices(); ++v)
   {
      if (getDegree(v) == 0) result.push_back(v);
   }
   return result;
}