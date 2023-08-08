#include <gtest/gtest.h>


#include "Indexer2.h"
#include <set>


TEST(Indexer2Test, ToFlat)
{
   ASSERT_EQ(Indexer2<int>::ToFlat(0, 0), 0);
   ASSERT_EQ(Indexer2<int>::ToFlat(0, 1), 1);
   ASSERT_EQ(Indexer2<int>::ToFlat(1, 1), 2);
   ASSERT_EQ(Indexer2<int>::ToFlat(1, 0), 3);

   ASSERT_EQ(Indexer2<int>::ToFlat(0, 2), 4);
   ASSERT_EQ(Indexer2<int>::ToFlat(1, 2), 5);
   ASSERT_EQ(Indexer2<int>::ToFlat(2, 2), 6);
   ASSERT_EQ(Indexer2<int>::ToFlat(2, 1), 7);
   ASSERT_EQ(Indexer2<int>::ToFlat(2, 0), 8);
}


TEST(Indexer2Test, FromFlat)
{
   ASSERT_EQ(Indexer2<int>::FromFlat(0).at(0), 0);
   ASSERT_EQ(Indexer2<int>::FromFlat(0).at(1), 0);

   ASSERT_EQ(Indexer2<int>::FromFlat(1).at(0), 0);
   ASSERT_EQ(Indexer2<int>::FromFlat(1).at(1), 1);

   ASSERT_EQ(Indexer2<int>::FromFlat(2).at(0), 1);
   ASSERT_EQ(Indexer2<int>::FromFlat(2).at(1), 1);

   ASSERT_EQ(Indexer2<int>::FromFlat(3).at(0), 1);
   ASSERT_EQ(Indexer2<int>::FromFlat(3).at(1), 0);

   ASSERT_EQ(Indexer2<int>::FromFlat(4).at(0), 0);
   ASSERT_EQ(Indexer2<int>::FromFlat(4).at(1), 2);

   ASSERT_EQ(Indexer2<int>::FromFlat(5).at(0), 1);
   ASSERT_EQ(Indexer2<int>::FromFlat(5).at(1), 2);
}



TEST(Indexer2Test, RoundTrip)
{
   const int np = 20;
   std::set<int> flats;
   for (int i = 0; i < np; ++i)
   {
      for (int j = 0; j < np; ++j)
      {
         const int flat = Indexer2<int>::ToFlat(i, j);
         flats.insert(flat);
         const auto rv = Indexer2<int>::FromFlat(flat);
         ASSERT_EQ(rv[0], i);
         ASSERT_EQ(rv[1], j);
      }
   }
   ASSERT_EQ(flats.size(), np * np);

}




