#include "Manifold0Reconstruction.h"
#include "MyException.h"

using namespace MeshGeneration;

void Reconstruction0::Check(MeshGeneration::NodeIndex manifoldPoint, const MeshGeneration::TrianglesNodes& trianglesNodes)
{
   if (!trianglesNodes.isKnownNodeId(manifoldPoint))
   {
      throw MyException("Manifold0Reconstruction::Check unknown node " + std::to_string(manifoldPoint));
   }
}

Manifold0Reconstruction::Manifold0Reconstruction(const Geometry::IManifoldId& manifoldId, const Reconstruction0& reconstruction) :
   m_manifold(manifoldId), m_reconstruction(reconstruction)
{
   if (m_reconstruction.node == NodeIndexInvalid)
   {
      throw MyException("Manifold0Reconstruction invalid node id");
   }
}

const Geometry::IManifoldId& Manifold0Reconstruction::getManifoldId() const
{
   return m_manifold;
}

const Reconstruction0& Manifold0Reconstruction::getReconstruction() const
{
   return m_reconstruction;
}