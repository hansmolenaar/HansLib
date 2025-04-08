#include "BoundsCheck.h"
#include "MyException.h"
#include "ReferenceShape.h"
#include "ReferenceShapePolygon.h"
#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"

#include <map>
#include <memory>

using namespace Topology;

namespace
{
   const auto Checker = BoundsCheck<int>::CreateLowerBound(3);
}

const IReferenceShape& ReferenceShapePolygon::Get(int numCorners)
{
   static std::map<int, std::unique_ptr<IReferenceShape>> m_polygons;
   if (m_polygons.find(numCorners) == m_polygons.end())
   {
      Checker.check(numCorners);
      const int numEdges = numCorners;
      std::map<int, std::vector< int>> edge2corners;
      for (int n = 0; n < numCorners; ++n)
      {
         edge2corners[n].push_back(n);
         edge2corners[n].push_back((n + 1) % numCorners);
      }
      std::map<int, std::vector< int>> face2edge;
      for (int n = 0; n < numEdges; ++n)
      {
         face2edge[0].push_back(n);
      }
      const TopologyDimension maxDimension = Face;
      std::map<TopologyDimension, int> counts{ {Corner,numCorners},{Edge,numEdges}, {Face,1} };
      std::vector<std::unique_ptr<ITopologicalAdjacency>> allAdjacencies;
      allAdjacencies.emplace_back(TopologicalAdjacency::Create(Edge, numEdges, Corner, numCorners, edge2corners));
      allAdjacencies.emplace_back(TopologicalAdjacency::CreateTrivial(Face, Edge, numEdges));
      allAdjacencies.emplace_back(TopologicalAdjacency::CreateTrivial(Face, Corner, numCorners));
      auto adjacencies = TopologicalAdjacencies::CreateWithPartialCounts(maxDimension, std::move(counts), std::move(allAdjacencies));
      m_polygons[numCorners] = std::make_unique< ReferenceShape>(std::move(adjacencies));
   }

   return *m_polygons.at(numCorners);
}

const IReferenceShape& ReferenceShapePolygon::GetTriangle()
{
   return Get(3);
}