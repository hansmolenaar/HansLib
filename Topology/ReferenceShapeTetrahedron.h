#pragma once

#include "IReferenceShape.h"
#include "TetrahedronNodesOriented.h"
#include "TopologicalAdjacencies.h"

namespace Topology
{
   class ReferenceShapeTetrahedron : public IReferenceShape
   {
   public:

      static const ReferenceShapeTetrahedron& getInstance();
      const ITopologicalAdjacencies& getAdjacencies() const override;

   private:
      ReferenceShapeTetrahedron();
      std::unique_ptr<TopologicalAdjacencies> m_adjacencies;
   };
}