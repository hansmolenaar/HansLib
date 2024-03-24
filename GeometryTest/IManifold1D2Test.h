#pragma once

#include <gtest/gtest.h>
#include "IManifold1D2.h"
#include "IManifold1Test.h"

using namespace Geometry;

template<typename T>
void IManifold1D2TestInterface(const IManifold1D2<T>& manifold, const IGeometryPredicate<T, IManifold1D2<T>::GeomDim>& predicate)
{
   constexpr int geomdim = IManifold1D2<T>::GeomDim;
   IManifold1TestInterface<T, geomdim>(manifold, predicate);

   const Point<T, geomdim> point{ 0,0 };
   const DirectedEdge<T, geomdim> edge(point, point);
   ASSERT_TRUE(edge.isDegenerate(predicate));
   ASSERT_THROW(manifold.GetIntersections(edge, predicate), MyException);
}

