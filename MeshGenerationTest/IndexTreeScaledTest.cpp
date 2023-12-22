#include <gtest/gtest.h>

#include "IndexTreeScaled.h"
#include "BoundingBox.h"

using namespace MeshGeneration;
using namespace IntervalTree;

TEST(IndexTreeScaledTest, ScaleCenter)
{
   constexpr int geomdim = 1;
   const BoundingBox<double, geomdim> bb = BoundingBox<double, geomdim>::CreateFromList(std::vector<Point1>{ Point1{ 1.0 }, Point1{ 2.0 }});
   const IndexTreeScaled<double, geomdim> tree(std::make_unique<IndexTree<geomdim>>(), bb);
   const auto center = tree.scaleCenter(tree.getTree().getRoot());
   ASSERT_NEAR(center.at(0), 1.5, 1.0e-10);
}

