#include "Manifold1Reconstruction.h"
#include "UndirectedGraph.h"

using namespace MeshGeneration;
using namespace Topology;

namespace
{

   std::set<Topology::EdgeNodesSorted> getEdges(const Boundary1 reconstruction)
   {
      std::set<Topology::EdgeNodesSorted> result;
      for (const auto& path : reconstruction.getPaths())
      {
         const size_t siz = path.size();
         for (size_t n = 1; n < siz; ++n)
         {
            result.emplace(path[n - 1], path[n]);
         }
      }

      for (const auto& cycle : reconstruction.getCycles())
      {
         const size_t siz = cycle.size();
         for (size_t n = 1; n <= siz; ++n)
         {
            result.emplace(cycle[n - 1], cycle[n % siz]);
         }
      }

      return result;
   }
}

Manifold1Reconstruction::Manifold1Reconstruction(const Geometry::IManifoldId& manifoldId, const Boundary1& reconstruction) :
   m_manifoldId(manifoldId), m_reconstruction(reconstruction), m_edges(getEdges(m_reconstruction))
{
}

Manifold1Reconstruction::Manifold1Reconstruction(const Geometry::IManifold1D2<GeomType>& manifold, const ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
   const TrianglesNodes& trianglesNodes) :
   m_manifoldId(manifold),
   m_reconstruction(Boundary1::createFromSubSet(manifoldsAndNodes.getNodesInManifold(&manifold), trianglesNodes)),
   m_edges(getEdges(m_reconstruction))
{
}

const Geometry::IManifoldId& Manifold1Reconstruction::getManifoldId() const
{
   return m_manifoldId;
}

const Boundary1& Manifold1Reconstruction::getReconstruction() const
{
   return m_reconstruction;
}

bool Manifold1Reconstruction::contains(const EdgeNodesSorted& edge) const
{
   return m_edges.contains(edge);
}