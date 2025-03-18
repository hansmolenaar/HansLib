#pragma once

#include "IGeometryRegion.h"
#include "IManifoldReconstruction.h"
#include "MeshGenerationDefines.h"

#include <memory>

class Logger;

namespace MeshGeneration
{
   class TrianglesNodes;

   void nibble(
      const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region,
      const std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>>& reconstructions,
      MeshGeneration::TrianglesNodes& trianglesNodes,
      const MeshGeneration::IUniquePointCollection2& pointCollection,
      Logger& logger);

}