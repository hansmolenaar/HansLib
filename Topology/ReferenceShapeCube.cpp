#include "MyException.h"
#include "ReferenceShapeCube.h"
#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"
#include "TopologyDefines.h"

using namespace Topology;

/// Local vertex ordering of the Hexahedron
///  
///    6-------------7
///   /|            /|
///  / |           / |
/// 4-------------5  |
/// |  |          |  |
/// |  2-------------3
/// | /           | /
/// |/            |/
/// 0-------------1
/// 
/// Local face ordering of the Hexahedron
/// 
/// 0:Front
/// 1:Back
/// 2:Left
/// 3:Right
/// 4:Bottom
/// 5:Top
/// 
/// 
/// Front:  0, 1, 5, 4
/// Back :  3, 2, 6, 7
/// Left :  2, 0, 4, 6
/// Right:  1, 3, 7, 5
/// Bottom: 1, 0, 2, 3
/// Top:    4, 5, 7, 6

namespace
{
   std::vector<std::unique_ptr<ITopologicalAdjacency>> createAdjacencies()
   {
      std::vector<std::unique_ptr<ITopologicalAdjacency>> result;
      result.emplace_back(TopologicalAdjacency::Create(Edge, NumEdgesOnCube, Corner, NumNodesOnCube,
         {
            { 0, { 0, 1 } },
            { 1, { 0, 2 } },
            { 2, { 0, 4 } },
            { 3, { 1, 3 } },
            { 4, { 1, 5 } },
            { 5, { 2, 3 } },
            { 6, { 2, 6 } },
            { 7, { 3, 7 } },
            { 8, { 4, 5 } },
            { 9, { 4, 6 } },
            { 10, { 5, 7 } },
            { 11, { 6, 7 } },
         }
         ));


      result.emplace_back(TopologicalAdjacency::Create(Face, NumFacesOnCube, Corner, NumNodesOnCube,
         {
           { 0, { 0, 1, 5, 4 } },
           { 1, { 2, 6, 7, 3 } },
           { 2, { 0, 4, 6, 2 } },
           { 3, { 1, 3, 7, 5 } },
           { 4, { 0, 2, 3, 1 } },
           { 5, { 4, 5, 7, 6 } },
         }
         ));

      result.emplace_back(TopologicalAdjacency::Create(Face, NumFacesOnCube, Edge, NumEdgesOnCube,
         {
           { 0, {0, 4, 8, 2 } },
           { 1, { 6, 11, 7, 5 } },
           { 2, { 2, 9, 6, 1  } },
           { 3, { 3, 7, 10, 4 } },
           { 4, { 1, 5, 3, 0} },
           { 5, { 8, 10, 11, 9 } },
         }
         ));

      result.emplace_back(TopologicalAdjacency::CreateTrivial(Volume, Corner, NumNodesOnCube));
      result.emplace_back(TopologicalAdjacency::CreateTrivial(Volume, Edge, NumEdgesOnCube));
      result.emplace_back(TopologicalAdjacency::CreateTrivial(Volume, Face, NumFacesOnCube));
      return result;
   }
}
const ITopologicalAdjacencies& ReferenceShapeCube::getAdjacencies() const
{
   return *m_adjacencies;
}

const ReferenceShapeCube& ReferenceShapeCube::getInstance()
{
   static ReferenceShapeCube instance;
   return instance;
}

ReferenceShapeCube::ReferenceShapeCube()
{
   const std::vector<int> counts{ NumNodesOnCube, NumEdgesOnCube, NumFacesOnCube, 1 };
   m_adjacencies = TopologicalAdjacencies::Create(counts, createAdjacencies());
}

const std::array<TetrahedronNodesOriented, ReferenceShapeCube::numTetsInStandardSplit>& ReferenceShapeCube::getStandardSplit() const
{
   static std::array<TetrahedronNodesOriented, numTetsInStandardSplit> split{
      TetrahedronNodesOriented(0, 3, 2, 7),
      TetrahedronNodesOriented(0, 2, 6, 7),
      TetrahedronNodesOriented(0, 6, 4, 7),
      TetrahedronNodesOriented(0, 5, 7, 4),
      TetrahedronNodesOriented(1, 5, 7, 0),
      TetrahedronNodesOriented(1, 7, 3, 0) };
   return split;
}