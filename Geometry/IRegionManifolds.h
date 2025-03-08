#pragma once

#include "IManifold.h"

namespace Geometry
{
   template<typename T, int N>
   class IRegionManifolds
   {
   public:
      virtual ~IRegionManifolds() noexcept = default;

      virtual  std::vector<const IManifold<T, N>*> getAllManifolds() const = 0;

      // Ignore lower dimensional manifolds
      virtual  std::vector<const IManifold<T, N>*> getBoundaryHyperManifolds() const = 0;

      // Adjacencies
      virtual std::vector<const IManifold<T, N>*> getConnectedLowers(const IManifold<T, N>& manifold) const = 0;
      virtual std::vector<const IManifold<T, N>*> getConnectedHighers(const IManifold<T, N>& manifold) const = 0;

      //std::vector<const IManifold<T, N>*> 
   };

}