#pragma once

#include "IReferenceShapeHyperCube.h"
#include "ITopologicalAdjacencies.h"

#include <memory>
#include <vector>

namespace Topology
{
   class GridTopology
   {
   public:
      explicit GridTopology(const std::vector<int>& cellDimensions);
      const ITopologicalAdjacencies& getAdjacencies() const;
      const IReferenceShapeHyperCube& getShape() const;

   private:
      std::unique_ptr<ITopologicalAdjacencies> m_adjacencies;
      const IReferenceShapeHyperCube& m_shape;
   };
}
