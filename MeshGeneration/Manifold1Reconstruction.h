#pragma once

#include "MeshGenerationDefines.h"
#include "TrianglesNodes.h"

namespace MeshGeneration
{
   class Reconstruction1
   {
   public:
      std::vector<NodeIndex> Singletons;
      std::vector<std::vector<NodeIndex>> Cycles;
      std::vector<std::vector<NodeIndex>> Paths;
   };

   Reconstruction1 Generate2(std::span<const NodeIndex> manifoldPoints, const TrianglesNodes& trianglesNodes, const IUniquePointCollecion2& pointCollection);
}