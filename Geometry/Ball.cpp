#include "Ball.h"

using namespace Geometry;

template Ball<int, 1>;
template Ball<int, 2>;
template Ball<int, 3>;
template Ball<double, 1>;
template Ball<double, 2>;
template Ball<double, 3>;

template<typename T, int N>
const IRegionManifolds<T, N>& Ball<T, N>::getManifolds() const
{
   throw MyException("Not yet implemented");
}