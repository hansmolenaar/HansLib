#pragma once

#include "IManifold1D2.h"
#include "IManifoldReconstruction.h"
#include "ManifoldsAndNodes.h"
#include "TrianglesNodes.h"

namespace MeshGeneration
{
   class Reconstruction1
   {
   public:
      std::vector<NodeIndex> Singletons;
      std::vector<std::vector<NodeIndex>> Cycles;
      std::vector<std::vector<NodeIndex>> Paths;
   };

   Reconstruction1 Generate2(std::span<const NodeIndex> manifoldPoints, const TrianglesNodes& trianglesNodes, const IUniquePointCollection2& pointCollection);

   class Manifold1Reconstruction : public IManifoldReconstruction
   {
   public:
      Manifold1Reconstruction(const Geometry::IManifoldId& manifoldId, const Reconstruction1& reconstruction);
      Manifold1Reconstruction(const Geometry::IManifold1D2<GeomType>& manifold, const ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes, const TrianglesNodes& trianglesNodes, const IUniquePointCollection2& pointCollection);

      const Geometry::IManifoldId& getManifoldId() const override;
      const Reconstruction1& getReconstruction() const;
   private:
      const Geometry::IManifoldId& m_manifoldId;
      const Reconstruction1 m_reconstruction;
   };
}