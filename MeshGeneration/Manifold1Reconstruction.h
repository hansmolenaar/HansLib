#pragma once

#include "EdgeNodesSorted.h"
#include "IManifold1D2.h"
#include "IManifoldReconstruction.h"
#include "ManifoldsAndNodes.h"
#include "TrianglesNodes.h"

#include <set>

namespace MeshGeneration
{
   class Reconstruction1
   {
   public:
      std::vector<Topology::NodeIndex> Singletons;
      std::vector<std::vector<Topology::NodeIndex>> Cycles;
      std::vector<std::vector<Topology::NodeIndex>> Paths;
   };

   Reconstruction1 Generate2(std::span<const Topology::NodeIndex> manifoldPoints, const TrianglesNodes& trianglesNodes, const IPointCollection2& pointCollection);

   class Manifold1Reconstruction : public IManifoldReconstruction
   {
   public:
      Manifold1Reconstruction(const Geometry::IManifoldId& manifoldId, const Reconstruction1& reconstruction);
      Manifold1Reconstruction(const Geometry::IManifold1D2<GeomType>& manifold, const ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes, const TrianglesNodes& trianglesNodes, const IPointCollection2& pointCollection);

      const Geometry::IManifoldId& getManifoldId() const override;
      const Reconstruction1& getReconstruction() const;
      bool contains(const Topology::EdgeNodesSorted& edge) const;
   private:
      const Geometry::IManifoldId& m_manifoldId;
      Reconstruction1 m_reconstruction;
      std::set<Topology::EdgeNodesSorted> m_edges;
   };
}