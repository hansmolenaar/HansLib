#include <gtest/gtest.h>

#include "Combinations.h"

#include <algorithm>
#include <set>

namespace str = std::ranges;

TEST(CombinationsTest, Test53)
{
   const auto allCombinations = Combinations::Get(5, 3);
   ASSERT_EQ(allCombinations.size(), 10);

   ASSERT_TRUE(str::all_of(allCombinations, [](const auto &cmb) { return cmb.size() == 5; }));

   std::set<std::vector<bool>> unique;
   for (const auto &bv : allCombinations)
   {
      unique.insert(bv);
   }
   ASSERT_EQ(unique.size(), 10);
}

TEST(CombinationsTest, Test24)
{
   const std::string data[] = {"HELLO", "WORLD", "ALPHA", "BET"};
   const int N = sizeof(data) / sizeof(*data);
   const auto allCombinations = Combinations::Get(N, 2);
   for (const auto &bv : allCombinations)
   {
      for (int n = 0; n < N; ++n)
      {
         if (bv.at(n))
         {
            std::cout << data[n] << " ";
         }
      }
      std::cout << "\n";
   }

   ASSERT_EQ(allCombinations.size(), 6);
}
