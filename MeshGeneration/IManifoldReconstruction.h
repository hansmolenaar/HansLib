#pragma once

#include "IManifoldId.h"
#include "MeshGenerationDefines.h"

namespace  MeshGeneration
{
   class IManifoldReconstruction
   {
   public:
      virtual ~IManifoldReconstruction() = default;

      virtual const Geometry::IManifoldId& getManifoldId() const = 0;
   };


}