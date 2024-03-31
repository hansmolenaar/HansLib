#pragma once

#include "IManifold.h"

namespace Geometry
{
   template<typename T, int N>
   class IRegionManifolds
   {
   public:
      virtual ~IRegionManifolds() noexcept = default;

      virtual  std::vector<const IManifold<T, N>*> GetAllManifolds() const = 0;

      // Ignore lower dimensional manifolds
      virtual  std::vector<const IManifold<T, N>*> GetBoundaryHyperManifolds() const = 0;

      // Adjacencies
      virtual std::vector<const IManifold<T, N>*> GetConnectedLowers(const IManifold<T, N>& manifold) const = 0;
      virtual std::vector<const IManifold<T, N>*> GetConnectedHighers(const IManifold<T, N>& manifold) const = 0;
   };

}