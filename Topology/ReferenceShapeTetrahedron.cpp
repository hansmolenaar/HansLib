#include "MyException.h"
#include "ReferenceShapeTetrahedron.h"
#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"
#include "TopologyDefines.h"

using namespace Topology;

// Local vertex ordering of the tetrahedron
//  
//      3
//    / | \
//   /  0   \
//  1 - ----  2

namespace
{
   std::vector<std::unique_ptr<ITopologicalAdjacency>> createAdjacencies()
   {
      std::vector<std::unique_ptr<ITopologicalAdjacency>> result;
      result.emplace_back(TopologicalAdjacency::Create(Edge, NumEdgesOnTetrahedron, Corner, NumNodesOnTetrahedron,
         {
            { 0, { 0, 1 } },
            { 1, { 0, 2 } },
            { 2, { 0, 3 } },
            { 3, { 1, 2 } },
            { 4, { 1, 3 } },
            { 5, { 2, 3 } }
         }
      ));


      result.emplace_back(TopologicalAdjacency::Create(Face, NumFacesOnTetrahedron, Corner, NumNodesOnTetrahedron,
         {
           { 0, { 0, 1, 3 } },
           { 1, { 0, 2, 1 } },
           { 2, { 0, 3, 2 } },
           { 3, { 1, 2, 3 } }
         }
      ));

      result.emplace_back(TopologicalAdjacency::Create(Face, NumFacesOnTetrahedron, Edge, NumEdgesOnTetrahedron,
         {
           { 0, {0, 4, 2 } },
           { 1, { 1, 3, 0 } },
           { 2, { 2, 5, 1  } },
           { 3, { 3, 5, 4 } }
         }
      ));

      result.emplace_back(TopologicalAdjacency::CreateTrivial(Volume, Corner, NumNodesOnTetrahedron));
      result.emplace_back(TopologicalAdjacency::CreateTrivial(Volume, Edge, NumEdgesOnTetrahedron));
      result.emplace_back(TopologicalAdjacency::CreateTrivial(Volume, Face, NumFacesOnTetrahedron));
      return result;
   }
}
const ITopologicalAdjacencies& ReferenceShapeTetrahedron::getAdjacencies() const
{
   return *m_adjacencies;
}

const ReferenceShapeTetrahedron& ReferenceShapeTetrahedron::getInstance()
{
   static ReferenceShapeTetrahedron instance;
   return instance;
}

ReferenceShapeTetrahedron::ReferenceShapeTetrahedron()
{
   const std::vector<int> counts{ NumNodesOnTetrahedron, NumEdgesOnTetrahedron, NumFacesOnTetrahedron, 1 };
   m_adjacencies = TopologicalAdjacencies::Create(counts, createAdjacencies());
}
