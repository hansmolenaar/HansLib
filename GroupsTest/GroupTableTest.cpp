#include <gtest/gtest.h>

#include "GroupTable.h"
#include "IFiniteGroupUtils.h"
#include "IndexerRowMajor.h"

TEST(GroupTable, Trivial)
{
   std::unique_ptr<IIndexer<GroupElement>> indexer = std::make_unique< IndexerRowMajor<GroupElement>>(1, 1);
   std::vector<GroupElement> table{ 0 };
   auto trivial = GroupTable::Create(indexer, table);
   ASSERT_EQ(trivial->getOrder(), 1);
   ASSERT_EQ(trivial->getIdentity(), 0);
   ASSERT_EQ(trivial->getInverse(0), 0);
   ASSERT_EQ((*trivial)(0, 0), 0);
}

TEST(GroupTable, Id2)
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


TEST(GroupTable, UnhappyPath)
{
   std::unique_ptr<IIndexer<GroupElement>> indexer = std::make_unique< IndexerRowMajor<GroupElement>>(2, 2);
   std::vector<GroupElement> table(4, 0);
   table.at(indexer->ToFlat({ 1,1 })) = 1;
   ASSERT_ANY_THROW(GroupTable::Create(indexer, table));
}


