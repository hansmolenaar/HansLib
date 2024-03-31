#pragma once

#include "MyException.h"



namespace Topology
{ 
   using TopologyDimension = unsigned int;
   constexpr TopologyDimension Corner = 0;
   constexpr TopologyDimension Edge = 1;
   constexpr TopologyDimension Face = 2;
   constexpr TopologyDimension Volume = 3;
}