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


TEST(IndexTreeScaledTest, ScalePoint)
{
   constexpr int geomdim = 2;
   const BoundingBox<double, geomdim> bb = BoundingBox<double, geomdim>::CreateFromList(std::vector<Point2>{ Point2{ 1.0, 2.0 }, Point2{ 2.0, 6.0 }});
   const IndexTreeScaled<double, geomdim> tree(std::make_unique<IndexTree<geomdim>>(), bb);

   auto result = tree.scalePoint(RatPoint2{ Rational(0, 1), Rational(0, 1) });
   ASSERT_NEAR(result.at(0), 1.0, 1.0e-10);
   ASSERT_NEAR(result.at(1), 2.0, 1.0e-10);

   result = tree.scalePoint(RatPoint2{ Rational(1, 1), Rational(1, 1) });
   ASSERT_NEAR(result.at(0), 2.0, 1.0e-10);
   ASSERT_NEAR(result.at(1), 6.0, 1.0e-10);

   result = tree.scalePoint(RatPoint2{ Rational(1, 2), Rational(1, 4) });
   ASSERT_NEAR(result.at(0), 1.5, 1.0e-10);
   ASSERT_NEAR(result.at(1), 3.0, 1.0e-10);
}

