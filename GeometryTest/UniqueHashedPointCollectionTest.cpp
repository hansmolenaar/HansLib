#include <gtest/gtest.h>

#include "UniqueHashedPointCollection.h"
#include "PointClose.h"

#include <numbers>

TEST(UniqueHashedPointCollectionTest, Basic)
{
   UniqueHashedPointCollection<int, 1> collection;
   ASSERT_EQ(collection.getNumPoints(), 0);
   IntPoint1 ipoint{ 42 };

   auto found = collection.tryGetClosePoint(ipoint);
   ASSERT_FALSE(std::get<0>(found));

   auto added = collection.addIfNew(ipoint);
   ASSERT_EQ(added, 0);
   ASSERT_EQ(collection.getNumPoints(), 1);
   found = collection.tryGetClosePoint(ipoint);
   ASSERT_TRUE(std::get<0>(found));
   ASSERT_EQ(std::get<1>(found), 0);
   const auto retval = collection.getPoint(0);
   collection.getGeometryPredicate().SamePoints(ipoint, retval);

   added = collection.addIfNew(ipoint);
   ASSERT_EQ(added, 0);
}


TEST(UniqueHashedPointCollectionTest, Rat2)
{
   UniqueHashedPointCollection<Rational, 2> collection;
   ASSERT_EQ(collection.getNumPoints(), 0);
   for (const auto& point :{ RatPoint2{1,2}, RatPoint2{1,3}, RatPoint2{2,1}, RatPoint2{1,3} })
   {
      collection.addIfNew(point);
   }
   ASSERT_EQ(collection.getNumPoints(), 3);
}
