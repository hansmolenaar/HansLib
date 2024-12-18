#include <gtest/gtest.h>

#include "GroupTable.h"
#include "IFiniteGroupUtils.h"
#include "IndexerRowMajor.h"

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
   const auto& [group, elements] = GroupTable::GeneratedBy(std::vector<Permutation>{Permutation::CreateFromCycle(2, std::vector<Permutation::Entry>{ 0,1 })});
   ASSERT_EQ(2, elements.size());
   ASSERT_EQ(2, group->getOrder());
}
