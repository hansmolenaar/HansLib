#include "MeshGenerationDefines.h"
#include "MeshingStrategy.h"
#include "Logger.h"

namespace MeshGeneration
{
   template<int N>
   class IMeshingSettings
   {
   public:
      virtual ~IMeshingSettings() = default;

      virtual  MeshingStrategy<N>& getStrategy() = 0;
      virtual MeshGeneration::CellQualityFun<N>* getMeshQuality() = 0;

      virtual Logger& getLogger() = 0;
   };

   using IMeshingSettings2 = IMeshingSettings<2>;
}