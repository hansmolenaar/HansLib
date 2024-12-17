#include <gtest/gtest.h>

#include "PermutationUtils.h" 

TEST(PermutationUtilsTest, Trivial)
{
   ASSERT_TRUE(PermutationUtils::IsPermutation(std::vector<int>{0, 1, 2, 3}));
}

TEST(PermutationUtilsTest, Basics)
{
   ASSERT_TRUE(PermutationUtils::IsPermutation(std::vector<int>{3, 1, 2, 0}));
   ASSERT_TRUE(PermutationUtils::IsPermutation(std::vector<int>{0}));
   ASSERT_FALSE(PermutationUtils::IsPermutation(std::vector<int>{}));
   ASSERT_FALSE(PermutationUtils::IsPermutation(std::vector<int>{2}));
   ASSERT_FALSE(PermutationUtils::IsPermutation(std::vector<int>{0, 1, 0}));
}


TEST(PermutationUtilsTest, Times)
{
   const Permutation permut = Permutation::Create(std::vector<int>{1, 0});
   const auto trivial = permut * permut;
   ASSERT_EQ(trivial.getCardinality(), 2);
   ASSERT_EQ(trivial(0), 0);
   ASSERT_EQ(trivial(1), 1);
}


TEST(PermutationUtilsTest, Equals)
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

TEST(PermutationUtilsTest, findIdentity)
{
   std::vector<Permutation> permutations;
   ASSERT_FALSE(PermutationUtils::findIdentity(permutations));
   permutations.emplace_back(Permutation::CreateFromCycle(5, std::vector<int>{2, 4}));
   ASSERT_FALSE(PermutationUtils::findIdentity(permutations));
   permutations.emplace_back(Permutation::CreateTrivial(5));
   ASSERT_EQ(1, *PermutationUtils::findIdentity(permutations));
}

TEST(PermutationUtilsTest, areUnique)
{
   std::vector<Permutation> permutations;
   ASSERT_TRUE(PermutationUtils::areUnique(permutations));

   permutations.emplace_back(Permutation::CreateTrivial(2));
   ASSERT_TRUE(PermutationUtils::areUnique(permutations));

   permutations.emplace_back(Permutation::CreateTrivial(7));
   ASSERT_TRUE(PermutationUtils::areUnique(permutations));

   permutations.emplace_back(Permutation::Create(std::vector<int>{0, 2, 6, 5, 4, 3, 1}));
   ASSERT_TRUE(PermutationUtils::areUnique(permutations));

   const Permutation c126 = Permutation::CreateFromCycle(7, std::vector<int>{1, 2, 6});
   permutations.emplace_back(c126);
   ASSERT_TRUE(PermutationUtils::areUnique(permutations));

   const Permutation c35 = Permutation::CreateFromCycle(7, std::vector<int>{3, 5});
   permutations.emplace_back(c35);
   ASSERT_TRUE(PermutationUtils::areUnique(permutations));

   permutations.emplace_back(c35 * c126);
   ASSERT_FALSE(PermutationUtils::areUnique(permutations));
}





