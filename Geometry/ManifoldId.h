#pragma once

#include "IManifoldId.h"


namespace Geometry
{
   // Testing only?
   class ManifoldId : public IManifoldId
   {
   public:
      ManifoldId(Topology::TopologyDimension dimension, const std::string& name);
      Topology::TopologyDimension getTopologyDimension() const override;
      const std::string& getName() const override;
   private:
      Topology::TopologyDimension m_dimension;
      std::string m_name;

   };


}
