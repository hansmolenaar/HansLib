#pragma once

#include "TopologyDimension.h"
#include "FeatureType.h"
#include <string>

class IModelFeature
{
public:
   virtual ~IModelFeature() = default;
   virtual std::string GetName() const = 0;
   virtual TopologyDimension GetTopologyDimension() const = 0;
   virtual FeatureType GetFeatureType() const = 0; 
}