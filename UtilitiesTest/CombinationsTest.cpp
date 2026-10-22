#include <gtest/gtest.h>

#include "Combinations.h"

#include <numeric>
#include <set>
#include <span>

using namespace Utilities;

namespace
{
void CheckCombinations(size_t N, size_t r, size_t expectedSize)
{
   ASSERT_EQ(countCombinations(N, r), expectedSize);
   std::vector<int> numbers(N);
   std::iota(numbers.begin(), numbers.end(), 0);
   std::set<std::vector<int>> all;
   CombinationS<int> combinations(numbers, r);

   int count = 0;
   do
   {
      // std::cout << combi[0] << " " << combi[1] << " " << combi[2] << "\n";
      all.insert(combinations());
      ++count;
   } while (combinations.next());

   ASSERT_EQ(count, expectedSize);
   ASSERT_EQ(all.size(), expectedSize);
}
} // namespace

TEST(CombinationsTest, Count)
{
   const size_t N = 5;
   ASSERT_EQ(countCombinations(N, 0), 1);
   ASSERT_EQ(countCombinations(N, 1), N);
   ASSERT_EQ(countCombinations(N, 2), 10);
   ASSERT_EQ(countCombinations(N, 3), 10);
   ASSERT_EQ(countCombinations(N, 4), 5);
   ASSERT_EQ(countCombinations(N, 5), 1);
   ASSERT_EQ(countCombinations(N, 6), 0);
}

TEST(CombinationsTest, EdgeCases)
{
   ASSERT_EQ(countCombinations(0, 0), 1);
   ASSERT_EQ(countCombinations(1, 0), 1);
   ASSERT_EQ(countCombinations(1, 1), 1);
   ASSERT_EQ(countCombinations(1, 2), 0);
}

TEST(CombinationsTest, Test53)
{
   std::set<std::vector<int>> all;
   CheckCombinations(5, 0, 1);
   CheckCombinations(5, 1, 5);
   CheckCombinations(5, 2, 10);
   CheckCombinations(5, 3, 10);
   CheckCombinations(5, 4, 5);
   CheckCombinations(5, 5, 1);
   ASSERT_ANY_THROW(CheckCombinations(5, 6, 0));
}

TEST(CombinationsTest, Test24)
{
   const std::vector<std::string> data{"HELLO", "WORLD", "ALPHA", "BET"};

   CombinationS<std::string> combinations(data, 2);
   ASSERT_EQ(combinations()[0], data[0]);
   ASSERT_EQ(combinations()[1], data[1]);
   int count = 0;
   do
   {
      ++count;
   } while (combinations.next());

   ASSERT_EQ(count, 6);
}
