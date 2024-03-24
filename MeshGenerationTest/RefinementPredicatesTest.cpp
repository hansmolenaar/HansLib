#include <gtest/gtest.h>

#include "IndexTreeScaled.h"
#include "RefinementPredicates.h"
#include "Sphere.h"
#include "IntervalTreeIndexFactory.h"
#include "PointClose.h"
#include "InitialBoundingboxGenerator.h"
#include "Ball2AsRegion.h"

using namespace MeshGeneration;
using namespace IntervalTree;
using namespace Geometry;

TEST(RefinementPredicatesTest, RefineRegionToMaxLevel_2)
{
   const Ball<double, 2> ball(Point2{ 5.0, 2.0 }, 1.0);
   const Ball2AsRegion ballAsRegion(ball);
   IndexFactory<2> factory;
   const auto initialBbGenerator = InitialBoundingboxGenerator<2>::Create(1.0);
   const PointClose<double, 2> areClose;
   const RefineRegionToMaxLevel<2> predicate(1, ballAsRegion, areClose, *initialBbGenerator);
   const auto* indexRoot = factory.getRoot();

   ASSERT_TRUE(predicate(*indexRoot));
}
