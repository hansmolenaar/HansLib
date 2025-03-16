#pragma once

#include "IManifoldReconstruction.h"
#include "MeshGenerationDefines.h"
#include "TrianglesNodes.h"

namespace  MeshGeneration
{
   class Reconstruction0
   {
   public:
      NodeIndex node = NodeIndexInvalid;

      void Check(MeshGeneration::NodeIndex manifoldPoint, const MeshGeneration::TrianglesNodes& trianglesNodes);
   };

   class Manifold0Reconstruction : public IManifoldReconstruction
   {
   public:
      Manifold0Reconstruction(const Geometry::IManifoldId& manifoldId, const Reconstruction0& reconstruction);
      const Geometry::IManifoldId& getManifoldId() const override;
      const Reconstruction0& getReconstruction0() const;
   private:
      const Geometry::IManifoldId& m_manifold;
      const Reconstruction0& m_reconstruction;
   };

}