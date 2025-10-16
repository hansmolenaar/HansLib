#include "IMeshingSettings.h"

using namespace Geometry;
using namespace MeshGeneration;

template class IMeshingSettings<2>;

template<size_t N>
BoundingBox<GeomType, N> IMeshingSettings<N>::getInitialBb(const Geometry::IGeometryRegion<GeomType, N>& region)
{
   return getInitialBbGenerator().generate(region);
}
