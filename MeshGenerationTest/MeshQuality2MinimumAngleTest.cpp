#include <gtest/gtest.h>

#include "CellQuality2MinimumAngle.h"


using namespace MeshGeneration;
using namespace Geometry;

TEST(MeshQuality2MinimumAngleTest, Creation)
{
   const CellQuality2MinimumAngle cellQuality;
   const Simplex<GeomType, GeomDim2> triangle{ Point2{0,0}, Point2{1,0}, Point2{0,1} };
   const double quality = cellQuality(triangle);
   ASSERT_NEAR( quality, 0.75, 1.0e-10);
}
