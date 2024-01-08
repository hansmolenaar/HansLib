#include <gtest/gtest.h>

#include "UniquePointCollectionBinning.h"
#include "PointClose.h"

#include <numbers>

TEST(UniquePointCollectionBinningTest, Empty)
{
   const PointClose<double, 1> predicate;
   const UniquePointCollectionBinning< 1> collection(predicate, std::vector<Point<double, 1>>{ Point1{ 1.0 }, Point1{ 2.0 }});
   ASSERT_EQ(collection.getNumPoints(), 2);
   const auto found = collection.tryGetClosePoint(Point<double, 1>{1.5});
   ASSERT_FALSE(std::get<0>(found));
}


TEST(UniquePointCollectionBinningTest, AddIfNew)
{
   const PointClose<double, 1> predicate;
   UniquePointCollectionBinning< 1> collection(predicate, std::vector<Point<double, 1>>{ Point1{ 1.0 }, Point1{ 2.0 }});
   ASSERT_EQ(collection.getNumPoints(), 2);

   const auto point0 = collection.getPoint(0);
   auto id = collection.addIfNew(point0);
   ASSERT_EQ(id, 0);

   const Point1 newPoint{ 1.5 };
   id = collection.addIfNew(newPoint);
   ASSERT_EQ(id, 2);
   ASSERT_TRUE(predicate.SamePoints(newPoint, collection.getPoint(id)));
}


TEST(UniquePointCollectionBinningTest, Delete)
{
   const PointClose<double, 1> predicate;
   UniquePointCollectionBinning< 1> collection(predicate, std::vector<Point<double, 1>>{ Point1{ 1.0 }, Point1{ 2.0 }});
   ASSERT_EQ(collection.getNumPoints(), 2);

   const auto point0 = collection.getPoint(0);
   const auto point1 = collection.getPoint(1);

   collection.deletePoint(0);
   ASSERT_EQ(collection.getNumPoints(), 1);

   ASSERT_TRUE(predicate.SamePoints(point1, collection.getPoint(1)));
   ASSERT_ANY_THROW(collection.getPoint(0));
}

TEST(UniquePointCollectionBinningTest, TryGetClosePoint1)
{
   const PointClose<double, 1> predicate;
   const UniquePointCollectionBinning< 1> collection(predicate, std::vector<Point<double, 1>>{ Point1{ 0.0 }, Point1{ 1.0 }});
   ASSERT_EQ(collection.getNumPoints(), 2);

   const Point1 nearPoint{ 0.1 * predicate.getSmallLengthInDirection(0) };
   const PointIndex expect = std::get<1>(collection.tryGetClosePoint(Point1{ 0 }));
   const auto [found, id] = collection.tryGetClosePoint(nearPoint);
   ASSERT_TRUE(found);
   ASSERT_EQ(id, expect);
}


TEST(UniquePointCollectionBinningTest, TryGetClosePoint2)
{
   const PointClose<double, 1> predicate;
   UniquePointCollectionBinning< 1> collection(predicate, std::vector<Point<double, 1>>{ Point1{ 0.0 }, Point1{ 1.0 }});
   const Point1 newPoint{ 0.1234 };
   const PointIndex expect = collection.addIfNew(newPoint);
   const Point1 nearPoint{ newPoint.at(0) + 0.1 * predicate.getSmallLengthInDirection(0) };
   const auto [found, id] = collection.tryGetClosePoint(nearPoint);
   ASSERT_TRUE(found);
   ASSERT_EQ(id, expect);
}

TEST(UniquePointCollectionBinningTest, TryGetClosePoint3)
{
   const PointClose<double, 1> predicate;
   const UniquePointCollectionBinning< 1> collection(predicate, std::vector<Point<double, 1>>{ Point1{ 0.0 }, Point1{ 1.0 }});

   // 2 close points in neighboring bins
   const auto& bins = collection.getBins(0);
   const double bound = bins.getBinUpper(1);
   const Point1 pointP{ bound + 0.1 * predicate.getSmallLengthInDirection(0) };
   const Point1 pointN{ bound - 0.1 * predicate.getSmallLengthInDirection(0) };
   const std::vector<Point1> points{pointP, pointN};

   for (size_t n0 = 0; n0 < points.size(); ++n0)
   {
      UniquePointCollectionBinning<1> collectionTest= collection;
      auto expect = collectionTest.addIfNew(points.at(n0));
      for (size_t n1 = 0; n1 < points.size(); ++n1)
      {
         auto found = collectionTest.tryGetClosePoint(points.at(n1));
         ASSERT_TRUE(std::get<0>(found));
         ASSERT_EQ(expect, std::get<1>(found));
      }    
   }
}


TEST(UniquePointCollectionBinningTest, TryGetClosePoint4)
{
   const PointClose<double, 2> predicate;
   const UniquePointCollectionBinning<2> collection(predicate, std::vector<Point<double, 2>>{ Point2{ 0,2 }, Point2{ 1, 3 }, Point2{ 1,2 }, Point2{ 0,3 }, Point2{0.2, 1.4}});

   // 2 close points in neighboring bins
   const auto& bins = collection.getBins(0);
   const double bound0 = collection.getBins(0).getBinUpper(1);
   const double bound1 = collection.getBins(1).getBinUpper(1);
   const double p0N = bound0 - 0.1 * predicate.getSmallLengthInDirection(0);
   const double p0P = bound0 + 0.1 * predicate.getSmallLengthInDirection(0);
   const double p1N = bound1 - 0.1 * predicate.getSmallLengthInDirection(1);
   const double p1P = bound1 + 0.1 * predicate.getSmallLengthInDirection(1);

   const std::vector<Point2> points{ Point2{p0N, p1N}, Point2{p0N, p1P}, Point2{p0P, p1N}, Point2{p0P, p1P} };

   for (size_t n0 = 0; n0 < points.size(); ++n0)
   {
      UniquePointCollectionBinning<2> collectionTest = collection;
      auto expect = collectionTest.addIfNew(points.at(n0));
      for (size_t n1 = 0; n1 < points.size(); ++n1)
      {
         auto found = collectionTest.tryGetClosePoint(points.at(n1));
         ASSERT_TRUE(std::get<0>(found));
         ASSERT_EQ(expect, std::get<1>(found));
      }
   }
}
