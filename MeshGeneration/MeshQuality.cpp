#include "MeshQuality.h"
#include "MyAssert.h"
#include "Simplex.h"
#include "TrianglesNodes.h"

using namespace MeshGeneration;
using namespace Topology;

namespace
{
   double GetCellQuality(const TriangleNodes& triangle, const IPointCollection<GeomType, GeomDim2>& points, CellQualityFun<GeomDim2>* qualityFunction)
   {
      const Geometry::Simplex<GeomType, GeomDim2> simplex{ points.getPoint(triangle[0]), points.getPoint(triangle[1]), points.getPoint(triangle[2]) };
      return (*qualityFunction)(simplex);
   }
}

double MeshQuality::getQuality2WholeMesh(const TrianglesNodes& triangles, const IPointCollection<GeomType, GeomDim2>& points, CellQualityFun<GeomDim2>* qualityFunction)
{
   return getQuality2(triangles, triangles.getAllTriangles(), points, qualityFunction);
}

double MeshQuality::getQuality2(
   const TrianglesNodes& triangles,
   std::span<const CellIndex> activeTriangles,
   const IPointCollection<GeomType, GeomDim2>& points,
   CellQualityFun<GeomDim2>* qualityFunction)
{
   Utilities::MyAssert(!activeTriangles.empty());

   double result = std::numeric_limits<double>::max();
   result = std::transform_reduce(
      activeTriangles.begin(), activeTriangles.end(),
      result,
      [](double a, double b) {return std::min(a, b); },
      [&triangles, &points, qualityFunction](CellIndex cell) {return GetCellQuality(triangles.getTriangleNodes(cell).asTriangleNodes(), points, qualityFunction); }
   );

   return result;
}