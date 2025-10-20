#include "Manifold1Reconstruction.h"

using namespace MeshGeneration;
using namespace Topology;

Manifold1Reconstruction::Manifold1Reconstruction(const Geometry::IManifoldId &manifoldId,
                                                 const Boundary1 &reconstruction)
    : m_manifoldId(manifoldId), m_reconstruction(reconstruction), m_edges(m_reconstruction.getEdges())
{
}

Manifold1Reconstruction::Manifold1Reconstruction(const Geometry::IManifold1D2<GeomType> &manifold,
                                                 const ManifoldsAndNodes<GeomDim2> &manifoldsAndNodes,
                                                 const TrianglesNodes &trianglesNodes)
    : m_manifoldId(manifold),
      m_reconstruction(Boundary1::createFromSubSet(manifoldsAndNodes.getNodesInManifold(&manifold), trianglesNodes)),
      m_edges(m_reconstruction.getEdges())
{
}

const Geometry::IManifoldId &Manifold1Reconstruction::getManifoldId() const
{
    return m_manifoldId;
}

const Boundary1 &Manifold1Reconstruction::getReconstruction() const
{
    return m_reconstruction;
}

bool Manifold1Reconstruction::contains(const EdgeNodesSorted &edge) const
{
    return m_edges.contains(edge);
}