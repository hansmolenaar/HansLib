#include "ReferenceShapeHyperCube.h"
#include "Utilities/Defines.h"
#include "Utilities/MyException.h"
#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"
#include "ReferenceShape.h"
#include "Utilities/BinomialCoefficient.h"
#include "Utilities/MultiIndex.h"

#include <set>

namespace
{

   std::unique_ptr<ITopologicalAdjacency> CreateEdge2Corner(TopologyDimension dim)
   {
      Utilities::Assert(dim > TopologyDimensionDef::Edge);
      const auto mi = MultiIndex<int>::Create(std::vector<int>(dim, 2));
      std::set<std::pair<int, int>> edges;
      for (auto crnr = 0; crnr < mi.getFlatSize(); ++crnr)
      {
         auto indx = mi.toMultiplet(crnr);
         for (TopologyDimension d = 0; d < dim; ++d)
         {
            // flip d-th bit
            indx.at(d) = (indx.at(d) ? 0 : 1);
            const auto flat = mi.toFlat(indx);
            edges.emplace(std::min(crnr, flat), std::max(crnr, flat));
            //flip back
            indx.at(d) = (indx.at(d) ? 0 : 1);
         }
      }

      std::map<int, std::vector<int>> edgeMap;
      int edgeId = 0;
      for (auto edge : edges)
      {
         edgeMap.emplace(edgeId, std::vector<int>{edge.first, edge.second });
         ++edgeId;
      }
      return TopologicalAdjacency::Create(TopologyDimensionDef::Edge, edgeId, TopologyDimensionDef::Corner, 1 << dim, edgeMap);
   }


   std::unique_ptr<ITopologicalAdjacency> CreateHyperFace2Corner(TopologyDimension dim)
   {
      Utilities::Assert(dim > TopologyDimensionDef::Edge);
      const int countHi = 2 * dim;
      const int countLo = 1 << dim;
      const auto mi = MultiIndex<int>::Create(std::vector<int>(dim, 2));
      std::map<int, std::vector<int>> faces;
      for (TopologyDimension d = 0; d < dim; ++d)
      {
         std::vector<int> faceL;
         std::vector<int> faceH;
         for (auto crnr = 0; crnr < countLo; ++crnr)
         {
            const auto multiplet = mi.toMultiplet(crnr);
            if (multiplet.at(d) == 0)
            {
               faceL.push_back(crnr);
            }
            else
            {
               faceH.push_back(crnr);
            }
         }
         std::ranges::sort(faceL);
         std::ranges::sort(faceH);
         faces.emplace(static_cast<int>(faces.size()), faceL);
         faces.emplace(static_cast<int>(faces.size()), faceH);
      }

      return TopologicalAdjacency::Create(dim - 1, countHi, TopologyDimensionDef::Corner, countLo, faces);
   }

   std::vector<std::unique_ptr<ITopologicalAdjacency>> CreateAdjecencyList(TopologyDimension dim, const std::vector<int>& counts)
   {
      std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacencyList;
      for (TopologyDimension d = 0; d < dim; ++d)
      {
         adjacencyList.emplace_back(TopologicalAdjacency::CreateTrivial(dim, d, counts.at(d)));
      }


      if (dim <= TopologyDimensionDef::Edge) return adjacencyList;
      if (dim <= TopologyDimensionDef::Volume)
      {
         adjacencyList.emplace_back(CreateHyperFace2Corner(dim));
         if (dim == TopologyDimensionDef::Face) return adjacencyList;
      }
      return adjacencyList;
   }

   std::unique_ptr<IReferenceShape> Create(TopologyDimension dim)
   {
      std::vector<int> counts;
      for (TopologyDimension m = 0; m <= dim; ++m)
      {
         counts.push_back((1 << (dim - m)) * BinomialCoefficient::Get(dim, m));
      }

      auto adjacencies = TopologicalAdjacencies::Create(counts, CreateAdjecencyList(dim, counts));
      auto referenceShape = std::make_unique<ReferenceShape>(std::move(adjacencies));
      return referenceShape;
   }

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