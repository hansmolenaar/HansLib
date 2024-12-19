#include <gtest/gtest.h>

#include "GroupTable.h"
#include "IFiniteGroupUtils.h"
#include "IndexerRowMajor.h"
#include "PermutationUtils.h"

#include <typeinfo>

TEST(GroupTableTest, Trivial)
{
   std::unique_ptr<IIndexer<GroupElement>> indexer = std::make_unique< IndexerRowMajor<GroupElement>>(1, 1);
   std::vector<GroupElement> table{ 0 };
   auto trivial = GroupTable::Create(indexer, table);
   ASSERT_EQ(trivial->getOrder(), 1);
   ASSERT_EQ(trivial->getIdentity(), 0);
   ASSERT_EQ(trivial->getInverse(0), 0);
   ASSERT_EQ((*trivial)(0, 0), 0);
}

TEST(GroupTableTest, Id2)
{
   std::unique_ptr<IIndexer<GroupElement>> indexer = std::make_unique< IndexerRowMajor<GroupElement>>(2, 2);
   std::vector<GroupElement> table(4, 1);
   table.at(indexer->ToFlat({ 1,0 })) = 0;
   table.at(indexer->ToFlat({ 0,1 })) = 0;
   auto trivial = GroupTable::Create(indexer, table);
   ASSERT_EQ(trivial->getOrder(), 2);
   ASSERT_EQ(trivial->getIdentity(), 1);
   ASSERT_EQ(trivial->getInverse(0), 0);
   ASSERT_EQ(trivial->getInverse(1), 1);
}


TEST(GroupTableTest, UnhappyPath)
{
   std::unique_ptr<IIndexer<GroupElement>> indexer = std::make_unique< IndexerRowMajor<GroupElement>>(2, 2);
   std::vector<GroupElement> table(4, 0);
   table.at(indexer->ToFlat({ 1,1 })) = 1;
   ASSERT_ANY_THROW(GroupTable::Create(indexer, table));
}

TEST(GroupTableTest, CreateFromPermutations_0)
{
   const auto& [group, elements] = GroupTable::GeneratedBy(std::vector<Permutation>{});
   ASSERT_TRUE(elements.empty());
   ASSERT_EQ(0, group->getOrder());
}

TEST(GroupTableTest, CreateFromPermutations_1)
{
   const auto& [group, elements] = GroupTable::GeneratedBy(std::vector<Permutation>{Permutation::CreateFromCycle(2, std::vector<Permutation::Entry>{ 0, 1 })});
   ASSERT_EQ(2, elements.size());
   ASSERT_EQ(2, group->getOrder());
   ASSERT_TRUE(PermutationUtils::isIdentity(elements.at(group->getIdentity())));
}

TEST(GroupTableTest, CreateFromPermutations_S3)
{
   const auto& [group, elements] = GroupTable::GeneratedBy(std::vector<Permutation>{
      Permutation::CreateFromCycle(3, std::vector<Permutation::Entry>{ 0, 1 }),
         Permutation::CreateFromCycle(3, std::vector<Permutation::Entry>{ 0, 1, 2 }) });
   ASSERT_EQ(6, elements.size());
   ASSERT_EQ(6, group->getOrder());
   ASSERT_TRUE(PermutationUtils::isIdentity(elements.at(group->getIdentity())));
}


TEST(GroupTableTest, CreateFromPermutations_S4)
{
   const auto& [group, elements] = GroupTable::GeneratedBy(std::vector<Permutation>{
      Permutation::CreateFromCycle(4, std::vector<Permutation::Entry>{ 0, 1 }),
         Permutation::CreateFromCycle(4, std::vector<Permutation::Entry>{ 0, 1, 2, 3 }) });
   ASSERT_EQ(24, elements.size());
   ASSERT_EQ(24, group->getOrder());
   ASSERT_TRUE(PermutationUtils::isIdentity(elements.at(group->getIdentity())));
}

TEST(GroupTableTest, CreateFromPermutations_5)
{
   const auto& [group, elements] = GroupTable::GeneratedBy(std::vector<Permutation>{Permutation::CreateTrivial(1)});
   ASSERT_EQ(1, elements.size());
   ASSERT_EQ(1, group->getOrder());
   ASSERT_TRUE(PermutationUtils::isIdentity(elements.at(group->getIdentity())));
}

TEST(GroupTableTest, CreateFromPermutations_D4)
{
   const auto& [group, elements] = GroupTable::GeneratedBy(std::vector<Permutation>{
      Permutation::CreateFromDisjunctCycles(4, { {0, 1}, {2,3} }),
         Permutation::CreateFromCycle(4, std::vector<Permutation::Entry>{ 0, 1, 2, 3 }) });
   ASSERT_EQ(8, elements.size());
   ASSERT_EQ(8, group->getOrder());
   ASSERT_TRUE(PermutationUtils::isIdentity(elements.at(group->getIdentity())));
}

TEST(GroupTableTest, CreateFromPermutations_D5)
{
   const auto& [group, elements] = GroupTable::GeneratedBy(std::vector<Permutation>{
      Permutation::CreateFromDisjunctCycles(5, { {1, 4}, {2,3} }),
         Permutation::CreateFromCycle(5, std::vector<Permutation::Entry>{ 0, 1, 2, 3, 4 }) });
   ASSERT_EQ(10, elements.size());
   ASSERT_EQ(10, group->getOrder());
   ASSERT_TRUE(PermutationUtils::isIdentity(elements.at(group->getIdentity())));
}

TEST(GroupTableTest, CreateUsingBinOp_0)
{
   auto mod2 = [](GroupElement n1, GroupElement n2) {return (n1 + n2) % 2; };
   using MyBinOp = decltype(mod2);

   const std::vector<GroupElement> elements{};
   auto result = GroupTable::CreateUsingBinaryOperator<MyBinOp>(elements, mod2, true);
   ASSERT_EQ(0, result->getOrder());
}

TEST(GroupTableTest, CreateUsingBinOp_1)
{
   auto mod2 = [](GroupElement n1, GroupElement n2) {return (n1 + n2) % 2; };
   using MyBinOp = decltype(mod2);

   const std::vector<GroupElement> elements{0};
   auto result = GroupTable::CreateUsingBinaryOperator<MyBinOp>(elements, mod2, true);
   ASSERT_EQ(1, result->getOrder());
}
