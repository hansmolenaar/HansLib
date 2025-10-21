#include "Mesh2.h"

using namespace MeshGeneration;
using namespace MeshGeneration2;
using namespace Topology;

void Mesh2::setBaseTriangles(const IndexTreeToSimplices2::Triangles &triangles)
{
    m_baseTriangles = triangles;
}

const IndexTreeToSimplices2::Triangles &Mesh2::getBaseTriangles() const
{
    return m_baseTriangles;
}

MeshGeneration::TrianglesNodes &Mesh2::getTriangles()
{
    return m_trianglesNodes;
}

const MeshGeneration::TrianglesNodes &Mesh2::getTriangles() const
{
    return m_trianglesNodes;
}

std::unique_ptr<IDynamicUniquePointCollection2> &Mesh2::createPoints()
{
    if (m_points)
    {
        throw MyException("Mesh2::setPoints already set");
    }
    return m_points;
}

const IUniquePointCollection2 &Mesh2::getPoints() const
{
    return *m_points;
}

IDynamicUniquePointCollection2 &Mesh2::getSetPoints()
{
    return *m_points;
}

const MeshGeneration::ManifoldsAndNodes<GeomDim2> &Mesh2::getManifoldsAndNodes() const
{
    return m_manifoldsAndNodes;
}

MeshGeneration::ManifoldsAndNodes<GeomDim2> &Mesh2::setManifoldsAndNodes()
{
    return m_manifoldsAndNodes;
}

void Mesh2::addReconstructions(std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>> &&reconstructions)
{
    m_reconstructions = std::move(reconstructions);
}

std::vector<const MeshGeneration::IManifoldReconstruction *> Mesh2::getReconstructions() const
{
    std::vector<const MeshGeneration::IManifoldReconstruction *> result(m_reconstructions.size());
    str::transform(m_reconstructions, result.begin(), [](const auto &up) { return up.get(); });
    return result;
}