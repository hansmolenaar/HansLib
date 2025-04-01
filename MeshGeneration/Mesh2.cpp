#include "Mesh2.h"

using namespace MeshGeneration;
using namespace MeshGeneration2;
using namespace Topology;

void MeshGeneration2::Mesh2::setBaseTriangles(const IndexTreeToSimplices2::Triangles& triangles)
{
   m_baseTriangles = triangles;
}

const IndexTreeToSimplices2::Triangles& Mesh2::getBaseTriangles() const
{
   return m_baseTriangles;
}

MeshGeneration::TrianglesNodes& MeshGeneration2::Mesh2::getTriangles()
{
   return m_trianglesNodes;
}

const MeshGeneration::TrianglesNodes& MeshGeneration2::Mesh2::getTriangles() const
{
   return m_trianglesNodes;
}
