#pragma once

#include "MeshingStrategy.h"
#include "MeshQuality.h"
#include "Logger.h"

namespace MeshGeneration
{
   template<int N>
   class MeshingSettings
   {
   public:
      MeshingSettings(std::unique_ptr<MeshingStrategy<N>>&& strategy, MeshQuality<N>&& quality );
      static std::unique_ptr<MeshingSettings> CreateDefault();
      const MeshingStrategy<N>& getStrategy() const;
      const MeshQuality<N>& getMeshQuality() const;
      Logger getLogger() const;

   private:
      std::unique_ptr<MeshingStrategy<N>>&& m_strategy;
      MeshQuality<N> m_quality;
      Logger m_logger;
   };

   using MeshingSettings2 = MeshingSettings<2>;
}