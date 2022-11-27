#pragma once

#include "Utilities/MyException.h"
enum class TopologyDimension { Corner, Edge, Face, Volume, };

inline TopologyDimension operator++ (TopologyDimension& d) {
   if (d == TopologyDimension::Volume) throw MyException("TopologyDimension out of bounds");
   d = static_cast<TopologyDimension>((static_cast<int>(d) + 1));
   return d;
}