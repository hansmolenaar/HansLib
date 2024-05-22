#include "IMeshingSettings.h"

using namespace Geometry;
using namespace MeshGeneration;

template IMeshingSettings<2>;

template<int N>
BoundingBox<GeomType, N> IMeshingSettings<N>::getInitialBb(const Geometry::IGeometryRegion<GeomType, N>& region)
{
   return getInitialBbGenerator().generate(region);
}