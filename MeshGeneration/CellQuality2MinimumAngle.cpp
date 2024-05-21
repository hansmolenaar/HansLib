#include "CellQuality2MinimumAngle.h"
#include "Triangle.h"
#include <numbers>

using namespace Geometry;
using namespace MeshGeneration;

namespace
{
   const CellQuality2MinimumAngle Instance;
}

double CellQuality2MinimumAngle::operator()(const Geometry::Simplex<GeomType, GeomDim2>& triangle) const
{
   const double smallestAngle = Triangle::SmallestAngle(triangle[0], triangle[1], triangle[2]);
   return 3 * smallestAngle / std::numbers::pi;
}

CellQuality2 CellQuality2MinimumAngle::GetCellQuality2()
{
   return Instance;
}