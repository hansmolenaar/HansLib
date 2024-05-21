#include <gtest/gtest.h>

#include "CellQuality2.h"


using namespace MeshGeneration;
using namespace Geometry;

TEST(MeshQuality2Test, Creation)
{
   const Simplex<GeomType, GeomDim2> triangle{ Point2{0,0}, Point2{1,0}, Point2{0,1} };
   const double quality = CellQuality2::MinimumAngle(triangle);
   ASSERT_NEAR( quality, 0.75, 1.0e-10);
}
