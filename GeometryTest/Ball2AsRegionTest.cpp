#include <gtest/gtest.h>

#include "Ball.h"
#include "PointClose.h"
#include "Ball2AsRegion.h"

using namespace Geometry;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(Ball2AsRegion, Constructor)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Ball<double, geomdim> ball({ 0,0 }, 1);
   const  Ball2AsRegion<double> region(ball);

   const auto bb = region.getBoundingBox();
   ASSERT_NEAR(bb.getMeasure(), 4.0, crit);

   ASSERT_TRUE(region.Contains({ 0.1, 0.2 }, areClose));
   ASSERT_TRUE(region.CouldIntersectWith(bb, areClose));

   const auto& regionManifolds = region.getManifolds();
   ASSERT_EQ(regionManifolds.GetAllManifolds().size(), 1);
   ASSERT_EQ(regionManifolds.GetBoundaryHyperManifolds().size(), 1);
   const auto* boundary = regionManifolds.GetBoundaryHyperManifolds().front();
   ASSERT_EQ(regionManifolds.GetConnectedLowers(*boundary).size(), 0);
   ASSERT_EQ(regionManifolds.GetConnectedHighers(*boundary).size(), 0);

}
