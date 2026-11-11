#include <gtest/gtest.h>

#include "BitsetUtils.h"

#include <algorithm>
#include <ranges>

using namespace Utilities;

namespace str = std::ranges;
namespace stv = std::ranges::views;

TEST(BitsetUtilsTest, Predicate)
{
   constexpr int size = 6;
   std::bitset<size> bits;
   bits.set(1);
   bits.set(4);

   const auto count = str::count_if(stv::iota(0, size), Bitset{bits});
   ASSERT_EQ(count, 2);

   int sum = 0;
   for (int i : std::views::iota(0, size) | std::views::filter(Bitset(bits)))
   {
      sum += i;
   }
   ASSERT_EQ(sum, 5);

   const auto first = (std::views::iota(0, 6) | std::views::filter(Bitset(bits))).front();
   ASSERT_EQ(first, 1);
}

TEST(BitsetUtilsTest, First)
{
   constexpr int size = 6;
   std::bitset<size> bits;
   bits.set(4);
   bits.set(5);

   ASSERT_EQ(Bitset{bits}.first(), 4);

   bits = std::bitset<size>();
   ASSERT_ANY_THROW(Bitset{bits}.first());
}
