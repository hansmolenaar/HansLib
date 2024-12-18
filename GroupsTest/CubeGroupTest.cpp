#include <gtest/gtest.h>

#include "CubeGroup.h"
#include "Permutation.h"
#include "PermutationUtils.h"

TEST(CubeGroupTest, getOrder)
{
   ASSERT_EQ(48, CubeGroup().getOrder());
}

TEST(CubeGroupTest, getIdentity)
{
   auto perm = Permutation::CreateTrivial(2);
   ASSERT_EQ(0, CubeGroup().getIdentity());
}

TEST(CubeGroupTest, getDistance)
{
   auto perm = Permutation::CreateTrivial(2);
   ASSERT_EQ(0, CubeGroup().getDistance(0, 0));
   ASSERT_EQ(0, CubeGroup().getDistance(7, 7));
   ASSERT_EQ(1, CubeGroup().getDistance(1, 3));
   ASSERT_EQ(1, CubeGroup().getDistance(2, 6));
   ASSERT_EQ(2, CubeGroup().getDistance(2, 7));
   ASSERT_EQ(2, CubeGroup().getDistance(1, 4));
   ASSERT_EQ(3, CubeGroup().getDistance(3, 4));
   ASSERT_EQ(3, CubeGroup().getDistance(2, 5));
}

TEST(CubeGroupTest, isIsometry)
{
   auto perm = Permutation::CreateTrivial(CubeGroup::numVertices);
   ASSERT_TRUE(CubeGroup().isIsometry(perm));
   perm = Permutation::CreateFromCycle(CubeGroup::numVertices, std::array<Permutation::Entry, 2>{0, 1});
   ASSERT_FALSE(CubeGroup::isIsometry(perm));

   for (const auto& perm : CubeGroup::getSymmetries())
   {
      ASSERT_TRUE(CubeGroup::isIsometry(perm));
   }
}

TEST(CubeGroupTest, getRotations)
{
   const size_t numRotations = 23;
   const auto rotations = CubeGroup::getRotations();
   ASSERT_EQ(numRotations, rotations.size());
   ASSERT_TRUE(PermutationUtils::areUnique(rotations));
}

TEST(CubeGroupTest, getReflections)
{
   const size_t numReflections = 9;
   const auto reflections = CubeGroup::getReflections();
   ASSERT_EQ(numReflections, reflections.size());
   ASSERT_TRUE(PermutationUtils::areUnique(reflections));
}


TEST(CubeGroupTest, getGroupSymmetries)
{
   const size_t numSymmetries = 48;
   const auto symmetries = CubeGroup::getGroupSymmetries();
   ASSERT_EQ(numSymmetries, symmetries.size());
   ASSERT_TRUE(PermutationUtils::areUnique(symmetries));
}

