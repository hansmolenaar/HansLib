#include <gtest/gtest.h>

#include "CellQuality2.h"


using namespace MeshGeneration;
using namespace Geometry;

TEST(CellQuality2Test, Creation)
{
   const Simplex<GeomType, GeomDim2> triangle{ Point2{0,0}, Point2{1,0}, Point2{0,1} };
   const double quality = CellQuality2::MinimumAngle(triangle);
   ASSERT_NEAR(quality, 0.75, 1.0e-10);
}


TEST(CellQuality2Test, FunctionPtr)
{
   const Simplex<GeomType, GeomDim2> triangle{ Point2{0,0}, Point2{1,0}, Point2{0,std::sqrt(3.0)}};
   CellQuality2Fun* fun = CellQuality2::MinimumAngle;
   const double quality = (*fun)(triangle);
   ASSERT_NEAR(quality, 0.5, 1.0e-10);
}
