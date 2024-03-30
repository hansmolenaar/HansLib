#pragma once

#include <gtest/gtest.h>
#include "IManifold1D2.h"
#include "IManifold1Test.h"

using namespace Geometry;

template<typename T>
void IManifold1D2TestInterface(const IManifold1D2<T>& manifold, const IGeometryPredicate<T, GeomDim2>& predicate)
{
   IManifold1TestInterface<T, GeomDim2>(manifold, predicate);

   const Point<T, GeomDim2> point{ 0,0 };
   const DirectedEdge<T, GeomDim2> edge(point, point);
   ASSERT_TRUE(edge.isDegenerate(predicate));
   ASSERT_THROW(manifold.GetIntersections(edge, predicate), MyException);
}

