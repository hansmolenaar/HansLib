#pragma once

#include "IManifoldReconstruction.h"
#include "IndexTreeToSimplices2.h"
#include "ManifoldsAndNodes.h"
#include "MeshGenerationDefines.h"
#include "TrianglesNodes.h"

namespace MeshGeneration2
{
class Mesh2
{
  public:
    void setBaseTriangles(const IndexTreeToSimplices2::Triangles &triangles);
    const IndexTreeToSimplices2::Triangles &getBaseTriangles() const;

    MeshGeneration::TrianglesNodes &getTriangles();
    const MeshGeneration::TrianglesNodes &getTriangles() const;

    std::unique_ptr<IDynamicUniquePointCollection2> &createPoints();
    const IUniquePointCollection2 &getPoints() const;
    IDynamicUniquePointCollection2 &getSetPoints();

    const MeshGeneration::ManifoldsAndNodes<GeomDim2> &getManifoldsAndNodes() const;
    MeshGeneration::ManifoldsAndNodes<GeomDim2> &setManifoldsAndNodes();

    void addReconstructions(std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>> &&reconstructions);
    std::vector<const MeshGeneration::IManifoldReconstruction *> getReconstructions() const;

  private:
    IndexTreeToSimplices2::Triangles m_baseTriangles;
    MeshGeneration::TrianglesNodes m_trianglesNodes;
    std::unique_ptr<IDynamicUniquePointCollection2> m_points;
    MeshGeneration::ManifoldsAndNodes<GeomDim2> m_manifoldsAndNodes;
    std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>> m_reconstructions;
};

} // namespace MeshGeneration2