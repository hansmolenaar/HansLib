#include <gtest/gtest.h>

#include "Ball.h"
#include "Ball2AsRegion.h"
#include "IGeometryRegionTest.h"
#include "PointClose.h"

using namespace Geometry;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(Ball2AsRegion, Constructor)
{
   const PointClose<double, GeomDim2> areClose;
   const Ball<double, GeomDim2> ball({ 0,0 }, 1);
   const  Ball2AsRegion<double> region(ball, "Ball2AsRegion_Constructor");

   IGeometryRegionTestInterface(region, areClose);

   const auto bb = region.getBoundingBox();
   ASSERT_NEAR(bb.getMeasure(), 4.0, crit);

   ASSERT_TRUE(region.contains({ 0.1, 0.2 }, areClose));
   ASSERT_TRUE(region.couldIntersectWith(bb, areClose));

   const auto& regionManifolds = region.getManifolds();
   ASSERT_EQ(regionManifolds.GetAllManifolds().size(), 1);
   ASSERT_EQ(regionManifolds.GetBoundaryHyperManifolds().size(), 1);
   const auto* boundary = regionManifolds.GetBoundaryHyperManifolds().front();
   ASSERT_EQ(regionManifolds.GetConnectedLowers(*boundary).size(), 0);
   ASSERT_EQ(regionManifolds.GetConnectedHighers(*boundary).size(), 0);

}
