#include "CellQuality3.h"
#include "Tetrahedron.h"
#include <numbers>

using namespace Geometry;
using namespace MeshGeneration;


double CellQuality3::SmallestDihedralAngle(const Geometry::Simplex<GeomType, GeomDim3>& tet)
{
   static const double normalization = 1 / std::acos(1.0 / 3.0);
   const double smallestAngle = Tetrahedron::getSmallestDihedralAngle(tet);
   return smallestAngle * normalization;
}