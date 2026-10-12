#include <gtest/gtest.h>

#include "Combinations.h"
#include "UniqueSorted.h"

#include <set>

using namespace Utilities;

TEST(CombinationsTest, Test53)
{
   std::set<std::vector<int>> all;
   const std::array<int, 5> data{1, 2, 3, 4, 5};
   std::vector<int> combi(3);
   auto generator = make_combination_generator(data.begin(), data.end(), combi.begin(), combi.end());
   int count = 0;
   do
   {
      UniqueSorted(combi);
      ASSERT_EQ(combi.size(), 3);
      // std::cout << combi[0] << " " << combi[1] << " " << combi[2] << "\n";
      all.insert(combi);
      ++count;
   } while (generator());

   ASSERT_EQ(count, 10);
   ASSERT_EQ(all.size(), 10);
}

TEST(CombinationsTest, Test24)
{
   const std::vector<std::string> data{"HELLO", "WORLD", "ALPHA", "BET"};
   std::vector<std::string> result(2);

   auto generator = make_combination_generator(data.begin(), data.end(), result.begin(), result.end());
   ASSERT_EQ(result[0], data[2]);
   ASSERT_EQ(result[1], data[3]);
   int count = 0;
   do
   {
      ++count;
   } while (generator());

   ASSERT_EQ(count, 6);
}
