#include "MeshStatistics.h"
#include "MeshQuality.h"

using namespace MeshGeneration;

MeshStatistics MeshStatistics::Create2(const TrianglesNodes& triangles, const IPointCollection<GeomType, GeomDim2>& points, CellQualityFun<GeomDim2>* qualityFunction)
{
   return {
      triangles.getAllNodes().size(),
      triangles.getAllTriangles().size(),
      MeshQuality::getQuality2WholeMesh(triangles, points, qualityFunction)
   };
}

bool MeshStatistics::operator==(const MeshStatistics& other) const
{
   constexpr double qualityCrit = 1.0e-3;
   if (this == &other) return true;
   if (NumberOfNodes != other.NumberOfNodes) return false;
   if (NumberOfCells != other.NumberOfCells) return false;
   if (std::abs(Quality - other.Quality) > qualityCrit) return false;
   return true;
}