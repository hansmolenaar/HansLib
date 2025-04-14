#pragma once

#include "Boundary1.h"
#include "EdgeNodesSorted.h"
#include "IManifold1D2.h"
#include "IManifoldReconstruction.h"
#include "ManifoldsAndNodes.h"
#include "TrianglesNodes.h"

#include <set>

namespace MeshGeneration
{

   class Manifold1Reconstruction : public IManifoldReconstruction
   {
   public:
      Manifold1Reconstruction(const Geometry::IManifoldId& manifoldId, const Boundary1& reconstruction);
      Manifold1Reconstruction(const Geometry::IManifold1D2<GeomType>& manifold, const ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes, const TrianglesNodes& trianglesNodes);

      const Geometry::IManifoldId& getManifoldId() const override;
      const Boundary1& getReconstruction() const;
      bool contains(const Topology::EdgeNodesSorted& edge) const;
   private:
      const Geometry::IManifoldId& m_manifoldId;
      Boundary1 m_reconstruction;
      std::set<Topology::EdgeNodesSorted> m_edges;
   };
}