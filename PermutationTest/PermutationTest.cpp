#include <gtest/gtest.h>

#include "Defines.h"
#include "Permutation.h"
#include "PermutationUtils.h"
#include "Single.h"

#include <array>

TEST(PermutaionTest, Trivial)
{
    const Permutation permut = Permutation::CreateTrivial(2);
    ASSERT_EQ(permut(0), 0);
    ASSERT_EQ(permut(1), 1);
    ASSERT_ANY_THROW(permut(-1));
    ASSERT_ANY_THROW(permut(2));
}

TEST(PermutaionTest, Trivial0)
{
    const Permutation permut = Permutation::CreateTrivial(0);
    ASSERT_EQ(permut.getCardinality(), 0);
}

TEST(PermutaionTest, Basic)
{
    const Permutation permut = Permutation::Create(std::vector<Permutation::Entry>{1, 0});
    ASSERT_EQ(permut.getCardinality(), 2);
    ASSERT_EQ(permut(0), 1);
    ASSERT_EQ(permut(1), 0);
}

TEST(PermutaionTest, FromInitializerList)
{
    const Permutation permut = Permutation::Create({1, 0});
    ASSERT_EQ(permut.getCardinality(), 2);
    ASSERT_EQ(permut(0), 1);
    ASSERT_EQ(permut(1), 0);
}

TEST(PermutaionTest, NotAPermutation)
{
    ASSERT_ANY_THROW(Permutation::Create({2, 0, 3}));
}

TEST(PermutaionTest, Random)
{
    const Permutation permut = Permutation::CreateTrivial(5);
    const auto shuffled0 = Permutation::CreateRandomShuffle(permut, 0);
    const auto shuffled1 = Permutation::CreateRandomShuffle(permut, 1);

    ASSERT_FALSE(permut == shuffled0);
    ASSERT_FALSE(permut == shuffled1);
    ASSERT_FALSE(shuffled0 == shuffled1);
}

TEST(PermutaionTest, FromCycle)
{
    const Permutation permut = Permutation::CreateFromCycle(3, std::vector<Permutation::Entry>{0, 2});
    ASSERT_EQ(permut.getCardinality(), 3);
    ASSERT_EQ(permut(0), 2);
    ASSERT_EQ(permut(1), 1);
    ASSERT_EQ(permut(2), 0);
}

TEST(PermutaionTest, FromCycle0)
{
    const Permutation permut = Permutation::CreateFromCycle(0, std::vector<Permutation::Entry>{});
    ASSERT_EQ(permut.getCardinality(), 0);
}

TEST(PermutaionTest, FromCycleProduct)
{
    const Permutation permut0 = Permutation::CreateFromCycle(10, std::vector<Permutation::Entry>{1, 3, 4, 7});
    const Permutation permut1 = Permutation::CreateFromCycle(10, std::vector<Permutation::Entry>{2, 6, 9});
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
    const Permutation permut0 = Permutation::CreateFromCycle(4, std::vector<Permutation::Entry>{2, 3});
    const Permutation permut1 = Permutation::CreateFromCycle(4, std::vector<Permutation::Entry>{0, 1, 2});
    const auto permut = permut1 * permut0;
    const Permutation permut_cycle = Permutation::CreateFromCycle(4, std::vector<Permutation::Entry>{0, 1, 3, 2});
    ASSERT_TRUE(permut == permut_cycle);
}

TEST(PermutaionTest, Apply_1)
{
    const Permutation permut = Permutation::Create(std::vector<Permutation::Entry>{1, 0});
    const std::array<bool, 2> in = {false, true};
    std::vector<bool> out(2);
    permut.apply(in.begin(), out.begin());
    ASSERT_TRUE(out.at(0));
    ASSERT_FALSE(out.at(1));
}

TEST(PermutaionTest, CreateFromDisjunctCycles_0)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(0, {});
    ASSERT_EQ(perm.getCardinality(), 0);
}

TEST(PermutaionTest, CreateFromDisjunctCycles_1)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(3, {{1, 2}});
    ASSERT_EQ(perm.getCardinality(), 3);
    const auto expect = Permutation::Create(std::vector<Permutation::Entry>{0, 2, 1});
    ASSERT_TRUE(expect == perm);
}

TEST(PermutaionTest, CreateFromDisjunctCycles_2)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(4, {{0, 1, 3}});
    ASSERT_EQ(perm.getCardinality(), 4);
    const auto expect = Permutation::Create(std::vector<Permutation::Entry>{1, 3, 2, 0});
    ASSERT_TRUE(expect == perm);
}

TEST(PermutaionTest, CreateFromDisjunctCycles_3)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(4, {{0, 2}, {1, 3}});
    ASSERT_EQ(perm.getCardinality(), 4);
    const auto expect = Permutation::Create(std::vector<Permutation::Entry>{2, 3, 0, 1});
    ASSERT_TRUE(expect == perm);
}

TEST(PermutaionTest, CreateFromDisjunctCycles_4)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(7, {{1, 2, 6}, {3, 5}});
    const auto expect = Permutation::Create(std::vector<Permutation::Entry>{0, 2, 6, 5, 4, 3, 1});
    ASSERT_TRUE(expect == perm);
}

TEST(PermutaionTest, spaceShip)
{
    const auto perm1 = Permutation::Create(std::vector<Permutation::Entry>{2, 3, 0, 1});
    const auto perm2 = Permutation::Create(std::vector<Permutation::Entry>{2, 3, 1, 0});
    ASSERT_TRUE(perm1 < perm2);
    ASSERT_TRUE(perm1 <= perm2);
    ASSERT_FALSE(perm1 > perm2);
    ASSERT_FALSE(perm1 >= perm2);
    ASSERT_FALSE(perm1 == perm2);
    ASSERT_TRUE(perm1 != perm2);
}

TEST(PermutaionTest, getInverse_0)
{
    const auto perm = Permutation::CreateTrivial(0);
    const auto inverse = perm.getInverse();
    ASSERT_EQ(0, inverse.getCardinality());
}

TEST(PermutaionTest, getInverse_1)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(7, {{1, 2, 6}, {3, 5}});
    const auto inverse = perm.getInverse();
    const auto id = perm * inverse;
    ASSERT_EQ(perm.getCardinality(), id.getCardinality());
    ASSERT_TRUE(PermutationUtils::isIdentity(id));
}

TEST(PermutaionTest, getInverse_2)
{
    const auto perm = Permutation::Create(std::vector<Permutation::Entry>{0, 5, 9, 1, 8, 2, 6, 4, 7, 3});
    const auto inverse = perm.getInverse();
    const auto expect = Permutation::Create(std::vector<Permutation::Entry>{0, 3, 5, 9, 7, 1, 6, 8, 4, 2});
    ASSERT_TRUE(inverse == expect);
}

TEST(PermutaionTest, getOrder_1)
{
    const auto perm = Permutation::CreateTrivial(7);
    ASSERT_EQ(perm.getOrder(), 1);
}

TEST(PermutaionTest, getOrder_2)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(5, {{0, 4, 1}, {2, 3}});
    ASSERT_EQ(perm.getOrder(), 6);
}

TEST(PermutaionTest, getOrder_3)
{
    const auto perm = Permutation::CreateFromCycle(6, {{0, 1, 2, 3, 4, 5}});
    ASSERT_EQ(perm.getOrder(), 6);
}

TEST(PermutaionTest, getPower_1)
{
    const auto perm = Permutation::CreateTrivial(7);
    ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(-2)));
    ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(-1)));
    ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(0)));
    ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(1)));
    ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(2)));
}

TEST(PermutaionTest, getPower_2)
{
    const auto perm = Permutation::CreateFromCycle(3, {{1, 2}});
    ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(-2)));
    ASSERT_FALSE(PermutationUtils::isIdentity(perm.getPower(-1)));
    ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(0)));
    ASSERT_FALSE(PermutationUtils::isIdentity(perm.getPower(1)));
    ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(2)));
}

TEST(PermutaionTest, getCycles_0)
{
    const auto perm = Permutation::CreateTrivial(0);
    const auto cycles = perm.getCycles();
    ASSERT_TRUE(cycles.empty());
    ASSERT_EQ(Permutation::Parity::EVEN, perm.getParity());
}

TEST(PermutaionTest, getCycles_1)
{
    const auto perm = Permutation::CreateTrivial(1);
    const auto cycles = perm.getCycles();
    ASSERT_TRUE(cycles.empty());
    ASSERT_EQ(Permutation::Parity::EVEN, perm.getParity());
}

TEST(PermutaionTest, getCycles_2)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(2, {{0, 1}});
    const auto cycles = perm.getCycles();
    ASSERT_EQ(1, cycles.size());
    ASSERT_EQ(2, cycles.front().size());
    ASSERT_EQ(Permutation::Parity::ODD, perm.getParity());
}

TEST(PermutaionTest, getCycles_3)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(10, {{0, 1}, {2, 5, 7}});
    const std::vector<std::vector<Permutation::Entry>> expect{{0, 1}, {2, 5, 7}};
    const auto cycles = perm.getCycles();
    ASSERT_EQ(2, cycles.size());
    ASSERT_TRUE(str::equal(cycles.at(0), expect.at(0)));
    ASSERT_EQ(Permutation::Parity::ODD, perm.getParity());
}

TEST(PermutaionTest, getCycles_4)
{
    const auto perm = Permutation::Create(std::vector<Permutation::Entry>{1, 5, 4, 3, 2, 0});
    const std::vector<std::vector<Permutation::Entry>> expect{{0, 1, 5}, {2, 4}};
    const auto cycles = perm.getCycles();
    ASSERT_EQ(2, cycles.size());
    ASSERT_TRUE(str::equal(cycles.at(0), expect.at(0)));
    ASSERT_TRUE(str::equal(cycles.at(1), expect.at(1)));
    ASSERT_EQ(Permutation::Parity::ODD, perm.getParity());
}

TEST(PermutaionTest, getCycles_5)
{
    const auto perm = Permutation::Create(std::vector<Permutation::Entry>{4, 0, 10, 9, 2, 3, 6, 1, 11, 5, 7, 8});
    const std::vector<std::vector<Permutation::Entry>> expect{{0, 4, 2, 10, 7, 1}, {3, 9, 5}, {8, 11}};
    const auto cycles = perm.getCycles();
    ASSERT_EQ(3, cycles.size());
    ASSERT_TRUE(str::equal(cycles.at(0), expect.at(0)));
    ASSERT_TRUE(str::equal(cycles.at(1), expect.at(1)));
    ASSERT_TRUE(str::equal(cycles.at(2), expect.at(2)));
    ASSERT_EQ(Permutation::Parity::EVEN, perm.getParity());
}

TEST(PermutaionTest, getCycles_6)
{
    const auto perm = Permutation::Create(std::vector<Permutation::Entry>{1, 5, 4, 3, 2, 0});
    const std::vector<std::vector<Permutation::Entry>> expect{{0, 1, 5}, {2, 4}};
    const auto cycles = perm.getCycles();
    ASSERT_EQ(2, cycles.size());
    ASSERT_TRUE(str::equal(cycles.at(0), expect.at(0)));
    ASSERT_TRUE(str::equal(cycles.at(1), expect.at(1)));

    std::vector<Permutation::Entry> vecin{1, 2, 3, 4, 5, 6};
    std::vector<Permutation::Entry> vecout(6);
    perm.apply(vecin.begin(), vecout.begin());
    const std::vector<Permutation::Entry> expectPerm{2, 6, 5, 4, 3, 1};
    ASSERT_TRUE(std::equal(vecout.begin(), vecout.end(), expectPerm.begin(), expectPerm.end()));
}

TEST(PermutaionTest, getParity_0)
{
    const auto perm = Permutation::Create(std::vector<Permutation::Entry>{1, 2, 0});
    ASSERT_EQ(Permutation::Parity::EVEN, perm.getParity());
}

TEST(PermutaionTest, getParity_1)
{
    const auto perm = Permutation::Create(std::vector<Permutation::Entry>{1, 2, 3, 0});
    ASSERT_EQ(Permutation::Parity::ODD, perm.getParity());
}

TEST(PermutaionTest, getParity_2)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(10, {{2, 5}, {7, 3}});
    ASSERT_EQ(Permutation::Parity::EVEN, perm.getParity());
}

TEST(PermutaionTest, getParity_3)
{
    const auto perm = Permutation::CreateFromDisjunctCycles(11, {{2, 5}, {7, 3}});
    ASSERT_EQ(Permutation::Parity::EVEN, perm.getParity());
}

TEST(PermutaionTest, getProduct0)
{
    const auto p1 = Permutation::CreateFromCycle(4, std::vector<Permutation::Entry>{0, 1});
    const auto p2 = Permutation::CreateFromCycle(4, std::vector<Permutation::Entry>{3, 2, 1});
    const std::vector<Permutation::Entry> vecIn{0, 1, 2, 3};
    std::vector<Permutation::Entry> vecOut(4);

    const auto p12 = p1 * p2;
    p12.apply(vecIn.begin(), vecOut.begin());
    const std::vector<Permutation::Entry> expect12{3, 0, 1, 2};
    ASSERT_TRUE(std::equal(vecOut.begin(), vecOut.end(), expect12.begin(), expect12.end()));

    const auto p21 = p2 * p1;
    p21.apply(vecIn.begin(), vecOut.begin());
    const std::vector<Permutation::Entry> expect21{1, 3, 0, 2};
    ASSERT_TRUE(std::equal(vecOut.begin(), vecOut.end(), expect21.begin(), expect21.end()));
}

TEST(PermutaionTest, example0)
{
    const auto perm = Permutation::CreateFromCycle(4, {{3, 2, 0}});
    const std::vector<Permutation::Entry> id{1, 2, 3, 4};
    std::vector<Permutation::Entry> rv(4);
    perm.apply(id.begin(), rv.begin());
    const std::vector<Permutation::Entry> expect{4, 2, 1, 3};
    ASSERT_TRUE(std::equal(rv.begin(), rv.end(), expect.begin(), expect.end()));

    const auto perm2 = Permutation::Create({3, 1, 0, 2});
    perm2.apply(id.begin(), rv.begin());
    ASSERT_TRUE(std::equal(rv.begin(), rv.end(), expect.begin(), expect.end()));
}
