#include "CellQuality3.h"
#include "Tetrahedron.h"
#include <numbers>

using namespace Geometry;
using namespace MeshGeneration;


double CellQuality3::SmallestDihedralAngle(const Geometry::Simplex<GeomType, GeomDim3>& tet)
{
   const double smallestAngle = 0; // Triangle::SmallestAngle(triangle[0], triangle[1], triangle[2]);
   return 3 * smallestAngle / std::numbers::pi;
}