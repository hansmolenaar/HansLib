#include <gtest/gtest.h>

#include "Permutation.h" 
#include "PermutationUtils.h"

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
   const Permutation permut_cycle = Permutation::CreateFromCycle(4, std::vector<Permutation::Entry>{0, 1, 2, 3});
   ASSERT_TRUE(permut == permut_cycle);
}


TEST(PermutaionTest, Apply_1)
{
   const Permutation permut = Permutation::Create(std::vector<Permutation::Entry>{1, 0});
   const std::array<bool, 2> in = { false, true };
   std::vector<bool> out(2);
   permut.apply(in.begin(), out.begin());
   ASSERT_TRUE(out.at(0));
   ASSERT_FALSE(out.at(1));
}

TEST(PermutaionTest, CreateFromDisjunctCycles_0)
{
   const auto perm = Permutation::CreateFromDisjunctCycles(0, { });
   ASSERT_EQ(perm.getCardinality(), 0);
}

TEST(PermutaionTest, CreateFromDisjunctCycles_1)
{
   const auto perm = Permutation::CreateFromDisjunctCycles(3, { { 1,2 } });
   ASSERT_EQ(perm.getCardinality(), 3);
   const auto expect = Permutation::Create(std::vector<Permutation::Entry>{0, 2, 1});
   ASSERT_TRUE(expect == perm);
}

TEST(PermutaionTest, CreateFromDisjunctCycles_2)
{
   const auto perm = Permutation::CreateFromDisjunctCycles(4, { { 0, 1, 3 } });
   ASSERT_EQ(perm.getCardinality(), 4);
   const auto expect = Permutation::Create(std::vector<Permutation::Entry>{1, 3, 2, 0});
   ASSERT_TRUE(expect == perm);
}

TEST(PermutaionTest, CreateFromDisjunctCycles_3)
{
   const auto perm = Permutation::CreateFromDisjunctCycles(4, { { 0, 2 }, {1, 3} });
   ASSERT_EQ(perm.getCardinality(), 4);
   const auto expect = Permutation::Create(std::vector<Permutation::Entry>{2, 3, 0, 1});
   ASSERT_TRUE(expect == perm);
}

TEST(PermutaionTest, CreateFromDisjunctCycles_4)
{
   const auto perm = Permutation::CreateFromDisjunctCycles(7, { { 1, 2, 6 }, {3, 5} });
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
   const auto perm = Permutation::CreateFromDisjunctCycles(7, { { 1, 2, 6 }, {3, 5} });
   const auto inverse = perm.getInverse();
   const auto id = perm * inverse;
   ASSERT_EQ(perm.getCardinality(), id.getCardinality());
   ASSERT_TRUE(PermutationUtils::isIdentity(id));
}

TEST(PermutaionTest, getInverse_2)
{
   const auto perm = Permutation::Create(std::vector<Permutation::Entry> {  0, 5, 9, 1, 8, 2, 6, 4, 7, 3 });
   const auto inverse = perm.getInverse();
   const auto expect = Permutation::Create(std::vector<Permutation::Entry> {  0, 3, 5, 9, 7, 1, 6, 8, 4, 2 });
   ASSERT_TRUE(inverse == expect);
}

TEST(PermutaionTest, getOrder_1)
{
   const auto perm = Permutation::CreateTrivial(7);
   ASSERT_EQ(perm.getOrder(), 1);
}

TEST(PermutaionTest, getOrder_2)
{
   const auto perm = Permutation::CreateFromDisjunctCycles(5, { {0, 4, 1}, {2, 3} });
   ASSERT_EQ(perm.getOrder(), 6);
}

TEST(PermutaionTest, getOrder_3)
{
   const auto perm = Permutation::CreateFromCycle(6, { {0, 1, 2, 3, 4, 5} });
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
   const auto perm = Permutation::CreateFromCycle(3, { {1, 2} });
   ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(-2)));
   ASSERT_FALSE(PermutationUtils::isIdentity(perm.getPower(-1)));
   ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(0)));
   ASSERT_FALSE(PermutationUtils::isIdentity(perm.getPower(1)));
   ASSERT_TRUE(PermutationUtils::isIdentity(perm.getPower(2)));
}