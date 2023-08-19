#include <gtest/gtest.h>

#include "UniquePointCollectionBinning.h"
#include "PointClose.h"

#include <numbers>

TEST(UniquePointCollectionBinningTest, Empty)
{
   const PointClose<double, 1> predicate;
   const UniquePointCollectionBinning< 1> collection(predicate, std::vector<Point<double, 1>>{ {1.0}, { 2.0 }});
   ASSERT_EQ(collection.getNumPoints(), 0);
 
}