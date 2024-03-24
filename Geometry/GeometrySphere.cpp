#include "GeometrySphere.h"

using namespace Geometry;

template Sphere<double, 1>;
template Sphere<double, 2>;
template Sphere<double, 3>;

template<typename T, int N>
const IRegionManifolds<T, N>& Sphere<T, N>::getManifolds() const
{
   throw MyException("Not yet implemented");
}