#include <gtest/gtest.h>

#include "Permutaion/PermutationUtils.h" 

TEST(PermutationUtils, Trivial)
{
   ASSERT_TRUE(PermutationUtils::IsPermutation(std::vector<int>{0, 1, 2, 3}));
}

TEST(PermutationUtils, Basics)
{
   ASSERT_TRUE(PermutationUtils::IsPermutation(std::vector<int>{3, 1, 2, 0}));
   ASSERT_TRUE(PermutationUtils::IsPermutation(std::vector<int>{0}));
   ASSERT_FALSE(PermutationUtils::IsPermutation(std::vector<int>{}));
   ASSERT_FALSE(PermutationUtils::IsPermutation(std::vector<int>{2}));
   ASSERT_FALSE(PermutationUtils::IsPermutation(std::vector<int>{0, 1, 0}));
}


TEST(PermutaionTest, Times)
{
   const Permutation permut = Permutation::Create(std::vector<int>{1, 0});
   const auto trivial = permut * permut;
   ASSERT_EQ(trivial.getCardinality(), 2);
   ASSERT_EQ(trivial(0), 0);
   ASSERT_EQ(trivial(1), 1);
}


TEST(PermutaionTest, Equals)
{
   const Permutation perm0 = Permutation::Create(std::vector<int>{1, 0});
   const Permutation perm1 = Permutation::Create(std::vector<int>{1, 0, 2});
   const Permutation perm2 = Permutation::Create(std::vector<int>{1, 2, 0});
   ASSERT_TRUE(perm0 == perm0);
   ASSERT_TRUE(perm1 == perm1);
   ASSERT_FALSE(perm0 == perm1);
   ASSERT_FALSE(perm1 == perm0);
   ASSERT_FALSE(perm2 == perm1);
}



