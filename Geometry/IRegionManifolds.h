#pragma once

#include "IManifold0.h"

namespace Geometry
{
   template<typename T, size_t N>
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

      template<typename Ttarget>
      std::vector<Ttarget> getManifoldsOfType() const;
   };

   template<typename T, size_t N>
   template<typename Ttarget>
   std::vector<Ttarget> IRegionManifolds<T, N>::getManifoldsOfType() const
   {
      std::vector<Ttarget> result;
      for (const auto* mptr : getAllManifolds())
      {
         auto mptrCasted = dynamic_cast<Ttarget>(mptr);
         if (mptrCasted != nullptr)
         {
            result.push_back(mptrCasted);
         }
      }
      return result;
   }
}
