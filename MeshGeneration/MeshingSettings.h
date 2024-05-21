#pragma once

#include "MeshGenerationDefines.h"
#include "MeshingStrategy.h"
#include "Logger.h"

namespace MeshGeneration
{
   template<int N>
   class MeshingSettings
   {
   public:
      MeshingSettings(std::unique_ptr<MeshingStrategy<N>>&& strategy /* , MeshGeneration::CellQuality<N> quality */ );
      //static std::unique_ptr<MeshingSettings<2>> CreateDefault2();
      const MeshingStrategy<N>& getStrategy() const;
      //const MeshGeneration::CellQuality<N>& getMeshQuality() const;
      Logger& getLogger();

   private:
      std::unique_ptr<MeshingStrategy<N>> m_strategy;
      //MeshGeneration::CellQuality<N> m_quality;
      Logger m_logger;
   };

   using MeshingSettings2 = MeshingSettings<2>;
}