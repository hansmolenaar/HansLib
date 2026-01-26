#include "IMeshingSettings.h"

namespace MeshGeneration
{
template class IMeshingSettings<2>;
}

using namespace Geometry;
using namespace MeshGeneration;

template <size_t N>
BoundingBox<GeomType, N> IMeshingSettings<N>::getInitialBb(const Geometry::IGeometryRegion<GeomType, N> &region)
{
    return getInitialBbGenerator().generate(region);
}
