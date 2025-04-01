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

std::unique_ptr<MeshGeneration::IUniquePointCollection2>& MeshGeneration2::Mesh2::createPoints()
{
   if (m_points)
   {
      throw MyException("Mesh2::setPoints already set");
   }
   return m_points;
}

const IPointCollection2& MeshGeneration2::Mesh2::getPoints() const
{
   return *m_points;
}

MeshGeneration::IUniquePointCollection2& MeshGeneration2::Mesh2::getPoints()
{
   return *m_points;
}