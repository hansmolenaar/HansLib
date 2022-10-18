#include <gtest/gtest.h>

#include "HLUtils/IndexerSymmetric.h"


namespace {
	void Check1(const IndexerSymmetric& indexer)
	{
		ASSERT_EQ(0, indexer.ToFlat(0, 0));
	}

	void Check2(const IndexerSymmetric& indexer)
	{
		Check1(indexer);
		ASSERT_EQ(1, indexer.ToFlat(0, 1));
		ASSERT_EQ(1, indexer.ToFlat(1, 0));
		ASSERT_EQ(2, indexer.ToFlat(1, 1));
	}

	void Check3(const IndexerSymmetric& indexer)
	{
		Check2(indexer);
		ASSERT_EQ(3, indexer.ToFlat(0, 2));
		ASSERT_EQ(4, indexer.ToFlat(1, 2));
		ASSERT_EQ(5, indexer.ToFlat(2, 2));

		ASSERT_EQ(3, indexer.ToFlat(2, 0));
		ASSERT_EQ(4, indexer.ToFlat(2, 1));
	}

	void Check4(const IndexerSymmetric& indexer)
	{
		Check3(indexer);
		ASSERT_EQ(6, indexer.ToFlat(0, 3));
		ASSERT_EQ(7, indexer.ToFlat(1, 3));
		ASSERT_EQ(8, indexer.ToFlat(2, 3));
		ASSERT_EQ(9, indexer.ToFlat(3, 3));

		ASSERT_EQ(6, indexer.ToFlat(3, 0));
		ASSERT_EQ(7, indexer.ToFlat(3, 1));
		ASSERT_EQ(8, indexer.ToFlat(3, 2));
	}

}
TEST(IndexerSymmetricTest, Test0)
{
	ASSERT_ANY_THROW(IndexerSymmetric(0); );
}


TEST(IndexerSymmetricTest, Test1)
{
	const IndexerSymmetric indexer(1);
	ASSERT_EQ(0, indexer.ToFlat(0, 0));
	ASSERT_EQ(0, indexer.ToFlat({ 0, 0 }));

	ASSERT_ANY_THROW(indexer.ToFlat({0}); );
	ASSERT_ANY_THROW(indexer.ToFlat({ 0,0,0 }); );
	ASSERT_ANY_THROW(indexer.ToFlat(-1,0); );
	ASSERT_ANY_THROW(indexer.ToFlat(0, -1); );
	ASSERT_ANY_THROW(indexer.ToFlat(1, 0); );
	ASSERT_ANY_THROW(indexer.ToFlat(0, 1); );
}


TEST(IndexerSymmetricTest, Test2)
{
	const IndexerSymmetric indexer(2);

	ASSERT_EQ(2, indexer.ToFlat({ 1, 1 }));
	Check2(indexer);
}


TEST(IndexerSymmetricTest, Test3)
{
	const IndexerSymmetric indexer(3);
	Check3(indexer);
}

TEST(IndexerSymmetricTest, Test4)
{
	const IndexerSymmetric indexer(4);
	Check4(indexer);
}