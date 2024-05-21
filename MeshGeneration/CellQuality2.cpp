#include "CellQuality2.h"
#include "Triangle.h"
#include <numbers>

using namespace Geometry;
using namespace MeshGeneration;


double CellQuality2::MinimumAngle(const Geometry::Simplex<GeomType, GeomDim2>& triangle)
{
   const double smallestAngle = Triangle::SmallestAngle(triangle[0], triangle[1], triangle[2]);
   return 3 * smallestAngle / std::numbers::pi;
}