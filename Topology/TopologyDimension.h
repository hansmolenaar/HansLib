#pragma once

#include "MyException.h"

using TopologyDimension = unsigned int;

namespace TopologyDimensionDef
{ 
   constexpr TopologyDimension Corner = 0;
   constexpr TopologyDimension Edge = 1;
   constexpr TopologyDimension Face = 2;
   constexpr TopologyDimension Volume = 3;
}