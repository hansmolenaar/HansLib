#pragma once

#include "Utilities/Defines.h"
#include "Groups/IFiniteGroup.h"
#include "Geometry/IPointTransformation.h"
#include "Geometry/IGeometryPredicate.h"
#include "Utilities/IndexerRowMajor.h"
#include "Groups/GroupTable.h"

namespace GroupFromOrbit
{
   // return empty result on failure
   template<int N>
   std::unique_ptr<IFiniteGroup> Create(const IGeometryPredicate<double, N>&, const std::vector<const IPointTransformation<double, N>*>&, const Point<double, N>&);
}

template<int N>
std::unique_ptr<IFiniteGroup> GroupFromOrbit::Create<N>(
   const IGeometryPredicate<double, N>& predicate,
   const std::vector<const IPointTransformation<double, N>*>& transformations,
   const Point<double, N>& trialPoint)
{
   std::vector<Point<double, N>> orbit;
   for (const auto& t : transformations)
   {
      const auto image = (*t)(trialPoint);
      auto finder = [&image, &predicate](const Point<double, N>& p) { return predicate.SamePoints(image, p); };
      if (std::find_if(orbit.begin(), orbit.end(), finder) != orbit.end()) return {};
      orbit.push_back(image);
   }

  
   const int groupSize = static_cast<int>(transformations.size());
   std::unique_ptr<IIndexer<GroupElement>> indexer = std::make_unique<IndexerRowMajor<int>>(groupSize, groupSize);
   std::vector< GroupElement> elements(groupSize*groupSize, -1);
   for (int g0 = 0; g0 < groupSize; ++g0)
   {
      for (int g1 = 0; g1 < groupSize; ++g1)
      {
         const auto image = (*transformations.at(g0))(orbit.at(g1));
         auto finder = [&image, &predicate](const Point<double, N>& p) { return predicate.SamePoints(image, p); };
         const auto found = std::find_if(orbit.begin(), orbit.end(), finder);
         if (found == orbit.end()) return {};
         const int g = static_cast<int>(std::distance(orbit.begin(), found));
         const int pos = indexer->ToFlat({g0,g1}); // TODO check order!
         elements.at(pos) = g;
      }
   }

   return GroupTable::Create(indexer, elements);
}