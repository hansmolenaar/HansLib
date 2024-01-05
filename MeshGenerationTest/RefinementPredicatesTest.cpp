#include <gtest/gtest.h>

#include "IndexTreeScaled.h"
#include "RefinementPredicates.h"
#include "GeometrySphere.h"
#include "IntervalTreeIndexFactory.h"
#include "PointClose.h"
#include "InitialBoundingboxGenerator.h"

using namespace MeshGeneration;
using namespace IntervalTree;
using namespace Geometry;

TEST(RefinementPredicatesTest, RefineRegionToMaxLevel_2)
{
   const Sphere<double, 2> sphere(Point2{ 5.0, 2.0 }, 1.0);
   IndexFactory<2> factory;
   const auto initialBbGenerator = InitialBoundingboxGenerator<2>::Create(1.0);
   const PointClose<double, 2> areClose;
   const RefineRegionToMaxLevel<2> predicate(1, sphere, areClose, *initialBbGenerator);
   const auto* indexRoot = factory.getRoot();

   ASSERT_TRUE(predicate(*indexRoot));
}
