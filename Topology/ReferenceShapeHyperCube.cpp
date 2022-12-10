#include "ReferenceShapeHyperCube.h"
#include "Utilities/Defines.h"
#include "Utilities/MyException.h"
#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"
#include "ReferenceShape.h"
#include "Utilities/BinomialCoefficient.h"

namespace
{
#if false
   std::unique_ptr<IReferenceShape> CreateEdge()
   {
      const std::map<int, std::vector<int>> hi2lo{ {0, std::vector<int>{0,1}, } };
      std::unique_ptr<ITopologicalAdjacency> adjacency = TopologicalAdjacency::Create(TopologyDimensionDef::Edge, 1, TopologyDimensionDef::Corner, 2, hi2lo);
      std::vector< std::unique_ptr<ITopologicalAdjacency>> adjacencyVector;
      adjacencyVector.push_back(std::move(adjacency));
      auto adjacencies = TopologicalAdjacencies::Create(std::vector<int>{2, 1}, std::move(adjacencyVector));
      auto referenceShape = std::make_unique<ReferenceShape>(std::move(adjacencies));
      return referenceShape;
   }
#endif

#if true
   std::unique_ptr<IReferenceShape> Create(TopologyDimension dim)
   {
      std::vector<int> counts;
      for (TopologyDimension m = 0; m <= dim; ++m)
      {
         auto tmp = 1 << (dim - m);
         auto chk = BinomialCoefficient::Get(dim, m);
         counts.push_back((1 << (dim - m)) * BinomialCoefficient::Get(dim, m));
      }

      auto adjacency = TopologicalAdjacency::CreateTrivial(dim, TopologyDimensionDef::Corner, 1 << dim);
      std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacencyList;
      adjacencyList.emplace_back(std::move(adjacency));
      auto adjacencies = TopologicalAdjacencies::Create(counts, std::move(adjacencyList));
      auto referenceShape = std::make_unique<ReferenceShape>(std::move(adjacencies));
      return referenceShape;
   }
#endif

   std::map<TopologyDimension, std::unique_ptr<IReferenceShape>> Shapes;
}

const IReferenceShape& ReferenceShapeHyperCube::Get(TopologyDimension dim)
{
   if (!Shapes.contains(dim))
   {
      Shapes.emplace(dim, Create(dim));
   }
   return *Shapes.at(dim);
}