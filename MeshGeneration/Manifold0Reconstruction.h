#pragma once

#include "IManifoldReconstruction.h"
#include "MeshGenerationDefines.h"
#include "TrianglesNodes.h"

namespace  MeshGeneration
{
   struct Reconstruction0
   {
      Topology::NodeIndex node = Topology::NodeIndexInvalid;

      void Check(Topology::NodeIndex manifoldPoint, const MeshGeneration::TrianglesNodes& trianglesNodes);
   };

   class Manifold0Reconstruction : public IManifoldReconstruction
   {
   public:
      Manifold0Reconstruction(const Geometry::IManifoldId& manifoldId, const Reconstruction0& reconstruction);
      const Geometry::IManifoldId& getManifoldId() const override;
      const Reconstruction0& getReconstruction() const;
   private:
      const Geometry::IManifoldId& m_manifold;
      const Reconstruction0& m_reconstruction;
   };

}