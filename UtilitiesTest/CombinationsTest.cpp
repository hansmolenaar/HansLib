#include <gtest/gtest.h>

#include "Combinations.h"
#include "UniqueSorted.h"

#include <numbers>
#include <ranges>
#include <set>
#include <span>

namespace str = std::ranges;

using namespace Utilities;

namespace
{
void CheckCombinations(size_t N, size_t r, size_t expectedSize)
{
   ASSERT_EQ(countCombinations(N, r), expectedSize);
   std::vector<int> numbers(N);
   str::copy(str::iota_view(static_cast<int>(1), static_cast<int>(N)), numbers.begin());
   std::vector<int> result(r);
   std::set<std::vector<int>> all;
   auto generator = make_combination_generator(numbers.begin(), numbers.end(), result.begin(), result.end());

   int count = 0;
   do
   {
      UniqueSorted(result);
      ASSERT_EQ(result.size(), r);
      // std::cout << combi[0] << " " << combi[1] << " " << combi[2] << "\n";
      all.insert(result);
      ++count;
   } while (generator());

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

TEST(CombinationsTest, BetterMethod)
{
   const std::vector<char> values{'a', 'b', 'c', 'd'};
   const int r = 2;
   CombinationS<char> combinations(values, r);
   do
   {
      std::cout << " BetterMethod";
      for (size_t n = 0; n < r; ++n)
      {
         std::cout << " " << combinations.values[n];
      }
      std::cout << "\n";
   } while (combinations.nextCombination());
}
