#pragma once

#include "FeatureType.h"
#include "TopologyDefines.h"
#include <string>

class IModelFeature
{
 public:
   virtual ~IModelFeature() = default;
   virtual std::string GetName() const = 0;
   virtual Topology::TopologyDimension getTopologyDimension() const = 0;
   virtual FeatureType GetFeatureType() const = 0;
}
