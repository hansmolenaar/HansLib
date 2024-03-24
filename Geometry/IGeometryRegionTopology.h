#pragma once

#include "TopologyDimension.h"
#include "IManifold.h"

namespace Geometry
{
   class IGeometryRegionTopology
   {
   public:
      virtual ~IGeometryRegionTopology() noexcept = default;

      virtual  std::vector<const IManifold*> GetAllManifolds() const = 0;

      // Ignore lower dimensional manifolds
      virtual  std::vector<const IManifold*> GetBoundaryHyperManifolds() const = 0;

      // Adjacencies
      virtual std::vector<const IManifold*> GetConnectedLowers(const IManifold& manifold) = 0;
      virtual std::vector<const IManifold*> GetConnectedHighers(const IManifold& manifold) = 0;
   };

}