#include <gtest/gtest.h>

#include "Permutation.h" 
#include "PermutationUtils.h"
TEST(PermutaionTest, Trivial)
{
   const Permutation permut = Permutation::CreateTrivial(2);
   ASSERT_EQ(permut(0), 0);
   ASSERT_EQ(permut(1), 1);
   ASSERT_ANY_THROW(permut(-1));
   ASSERT_ANY_THROW(permut(2));
   ASSERT_ANY_THROW(Permutation::CreateTrivial(0));
}


TEST(PermutaionTest, Basic)
{
   const Permutation permut = Permutation::Create(std::vector<int>{1, 0});
   ASSERT_EQ(permut.getCardinality(), 2);
   ASSERT_EQ(permut(0), 1);
   ASSERT_EQ(permut(1), 0);
}


TEST(PermutaionTest, FromCycle)
{
   const Permutation permut = Permutation::CreateFromCycle(3, std::vector<int>{0, 2});
   ASSERT_EQ(permut.getCardinality(), 3);
   ASSERT_EQ(permut(0), 2);
   ASSERT_EQ(permut(1), 1);
   ASSERT_EQ(permut(2), 0);
}

TEST(PermutaionTest, FromCycleProduct)
{
   const Permutation permut0 = Permutation::CreateFromCycle(10, std::vector<int>{1, 3, 4, 7});
   const Permutation permut1 = Permutation::CreateFromCycle(10, std::vector<int>{2, 6, 9});
   const auto permut = permut0 * permut1;
   ASSERT_EQ(permut.getCardinality(), 10);
   ASSERT_EQ(permut(0), 0);
   ASSERT_EQ(permut(1), 3);
   ASSERT_EQ(permut(2), 6);
   ASSERT_EQ(permut(3), 4);
   ASSERT_EQ(permut(4), 7);
   ASSERT_EQ(permut(5), 5);
   ASSERT_EQ(permut(6), 9);
   ASSERT_EQ(permut(7), 1);
   ASSERT_EQ(permut(8), 8);
   ASSERT_EQ(permut(9), 2);
}


TEST(PermutaionTest, TetSymmetry)
{
   const Permutation permut0 = Permutation::CreateFromCycle(4, std::vector<int>{2, 3});
   const Permutation permut1 = Permutation::CreateFromCycle(4, std::vector<int>{0, 1, 2});
   const auto permut = permut1 * permut0;
   const Permutation permut_cycle = Permutation::CreateFromCycle(4, std::vector<int>{0, 1, 2, 3});
   ASSERT_TRUE(permut == permut_cycle);
}

