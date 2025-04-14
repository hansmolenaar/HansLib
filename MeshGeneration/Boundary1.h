#pragma once

#include "EdgeNodesSorted.h"
#include "IManifold1D2.h"
#include "IManifoldReconstruction.h"
#include "ManifoldsAndNodes.h"
#include "TrianglesNodes.h"

#include <set>

namespace MeshGeneration
{
   class Boundary1
   {
   public:
      std::vector<Topology::NodeIndex> Singletons;
      std::vector<std::vector<Topology::NodeIndex>> Cycles;
      std::vector<std::vector<Topology::NodeIndex>> Paths;
   };

}