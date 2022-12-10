#pragma once

#include "Utilities/MyException.h"

using TopologyDimension = unsigned int;

namespace TopologyDimensionDef
{ 
   using TopologyDimension = unsigned int;

   constexpr TopologyDimension Corner = 0;
   constexpr TopologyDimension Edge = 1;
   constexpr TopologyDimension Face = 2;
   constexpr TopologyDimension Volume = 3;
}