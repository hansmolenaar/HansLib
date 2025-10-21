#include <gtest/gtest.h>

#include "Defines.h"
#include "PermutationUtils.h"

namespace
{
// 3 --- 2
// |     |
// 0 --- 1

struct DistanceSquare
{
    int operator()(Permutation::Entry v0, Permutation::Entry v1)
    {
        if (v0 == v1)
            return 0;
        if (v0 + 2 == v1)
            return 2;
        if (v1 + 2 == v0)
            return 2;
        return 1;
    }
};

} // namespace
TEST(PermutationUtilsTest, Trivial)
{
    ASSERT_TRUE(PermutationUtils::IsPermutation(std::vector<Permutation::Entry>{0, 1, 2, 3}));
}

TEST(PermutationUtilsTest, Basics)
{
    ASSERT_TRUE(PermutationUtils::IsPermutation(std::vector<Permutation::Entry>{3, 1, 2, 0}));
    ASSERT_TRUE(PermutationUtils::IsPermutation(std::vector<Permutation::Entry>{0}));
    ASSERT_TRUE(PermutationUtils::IsPermutation(std::vector<Permutation::Entry>{}));
    ASSERT_FALSE(PermutationUtils::IsPermutation(std::vector<Permutation::Entry>{2}));
    ASSERT_FALSE(PermutationUtils::IsPermutation(std::vector<Permutation::Entry>{0, 1, 0}));
}

TEST(PermutationUtilsTest, Times)
{
    const Permutation permut = Permutation::Create(std::vector<Permutation::Entry>{1, 0});
    const auto trivial = permut * permut;
    ASSERT_EQ(trivial.getCardinality(), 2);
    ASSERT_EQ(trivial(0), 0);
    ASSERT_EQ(trivial(1), 1);
}

TEST(PermutationUtilsTest, Equals)
{
    const Permutation perm0 = Permutation::Create(std::vector<Permutation::Entry>{1, 0});
    const Permutation perm1 = Permutation::Create(std::vector<Permutation::Entry>{1, 0, 2});
    const Permutation perm2 = Permutation::Create(std::vector<Permutation::Entry>{1, 2, 0});
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
    permutations.emplace_back(Permutation::CreateFromCycle(5, std::vector<Permutation::Entry>{2, 4}));
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

    permutations.emplace_back(Permutation::Create(std::vector<Permutation::Entry>{0, 2, 6, 5, 4, 3, 1}));
    ASSERT_TRUE(PermutationUtils::areUnique(permutations));

    const Permutation c126 = Permutation::CreateFromCycle(7, std::vector<Permutation::Entry>{1, 2, 6});
    permutations.emplace_back(c126);
    ASSERT_TRUE(PermutationUtils::areUnique(permutations));

    const Permutation c35 = Permutation::CreateFromCycle(7, std::vector<Permutation::Entry>{3, 5});
    permutations.emplace_back(c35);
    ASSERT_TRUE(PermutationUtils::areUnique(permutations));

    permutations.emplace_back(c35 * c126);
    ASSERT_FALSE(PermutationUtils::areUnique(permutations));
}

TEST(PermutationUtilsTest, generateAllPowerCombinations_0)
{
    const std::set<Permutation> permutations =
        PermutationUtils::generateAllPowerCombinations(std::vector<Permutation>{});
    ASSERT_TRUE(permutations.empty());
}

TEST(PermutationUtilsTest, generateAllPowerCombinations_1)
{
    const std::vector<Permutation> generators{Permutation::CreateFromCycle(2, std::vector<Permutation::Entry>{0, 1})};
    const std::set<Permutation> permutations = PermutationUtils::generateAllPowerCombinations(generators);
    ASSERT_EQ(2, permutations.size());
    ASSERT_EQ(1, str::count_if(permutations, PermutationUtils::isIdentity));
}

TEST(PermutationUtilsTest, CreateFromPermutations_A5)
{
    const std::vector<Permutation> generators{
        Permutation::CreateFromCycle(5, std::vector<Permutation::Entry>{0, 1, 2, 3, 4}),
        Permutation::CreateFromCycle(5, std::vector<Permutation::Entry>{0, 1, 2})};
    const std::set<Permutation> permutations = PermutationUtils::generateAllPowerCombinations(generators);
    ASSERT_EQ(60, permutations.size());
    ASSERT_EQ(1, str::count_if(permutations, PermutationUtils::isIdentity));
}

TEST(PermutationUtilsTest, CreateFromPermutations_A6)
{
    const std::vector<Permutation> generators{
        Permutation::CreateFromCycle(6, std::vector<Permutation::Entry>{1, 2, 3, 4, 5}),
        Permutation::CreateFromCycle(6, std::vector<Permutation::Entry>{0, 1, 2})};

    const std::set<Permutation> permutations = PermutationUtils::generateAllPowerCombinations(generators);
    ASSERT_EQ(360, permutations.size());
    ASSERT_EQ(1, str::count_if(permutations, PermutationUtils::isIdentity));
}

TEST(PermutationUtilsTest, IsIsometry)
{
    Permutation permutation = Permutation::CreateTrivial(4);
    ASSERT_TRUE(PermutationUtils::isIsometry(permutation, DistanceSquare()));

    permutation = Permutation::CreateFromDisjunctCycles(4, {{0, 1, 2, 3}});
    ASSERT_TRUE(PermutationUtils::isIsometry(permutation, DistanceSquare()));

    permutation = Permutation::CreateFromDisjunctCycles(4, {{0, 2}});
    ASSERT_TRUE(PermutationUtils::isIsometry(permutation, DistanceSquare()));

    permutation = Permutation::CreateFromDisjunctCycles(4, {{0, 1}});
    ASSERT_FALSE(PermutationUtils::isIsometry(permutation, DistanceSquare()));
}

TEST(PermutationUtilsTest, CastToEntry_0)
{
    constexpr int number = 2;
    constexpr Permutation::Entry expect = 2;
    ASSERT_EQ(expect, PermutationUtils::SafeCastToEntry(number));
}

TEST(PermutationUtilsTest, CastToEntry_1)
{
    constexpr int number = -1;
    ASSERT_MYEXCEPTION_MESSAGE(PermutationUtils::SafeCastToEntry(number), "CastToEntry negative number -1");
}

TEST(PermutationUtilsTest, CastToEntry_2)
{
    constexpr size_t number = std::numeric_limits<size_t>::max();
    ASSERT_MYEXCEPTION_MESSAGE(PermutationUtils::SafeCastToEntry(number), "CastToEntry too large number");
}