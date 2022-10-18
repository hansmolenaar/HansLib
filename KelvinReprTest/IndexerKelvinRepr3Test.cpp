#include <gtest/gtest.h>

#include "KelvinRepr/IndexerKelvinRepr3.h"


TEST(IndexerKelvinRepr3Test, Test1)
{
	const IndexerKelvinRepr3 indexer;
	ASSERT_EQ(indexer.ToFlat({ 0, 0 }), 0);
	ASSERT_EQ(indexer.ToFlat({ 1, 1 }), 1);
	ASSERT_EQ(indexer.ToFlat({ 2, 2 }), 2);

	ASSERT_EQ(indexer.ToFlat({ 1, 2 }), 3);
	ASSERT_EQ(indexer.ToFlat({ 2, 1 }), 3);

	ASSERT_EQ(indexer.ToFlat({ 0, 2 }), 4);
	ASSERT_EQ(indexer.ToFlat({ 2, 0 }), 4);

	ASSERT_EQ(indexer.ToFlat({ 0, 1 }), 5);
	ASSERT_EQ(indexer.ToFlat({ 1, 0 }), 5);
}


TEST(IndexerKelvinRepr3Test, TestErrors)
{
	const IndexerKelvinRepr3 indexer;
	ASSERT_ANY_THROW(indexer.ToFlat({ -1, 0 }));
	ASSERT_ANY_THROW(indexer.ToFlat({ 0, -1 }));
	ASSERT_ANY_THROW(indexer.ToFlat({ 3, 0 }));
	ASSERT_ANY_THROW(indexer.ToFlat({ 0, 3 }));
	ASSERT_ANY_THROW(indexer.ToFlat({ -1, -1 }));
	ASSERT_ANY_THROW(indexer.ToFlat({ 3, 3 }));
}

TEST(IndexerKelvinRepr3Test, Tet2RowCol)
{
   const IndexerKelvinRepr3 indexer;
   for (int flat = 0; flat < 6; ++flat)
   {
      int row;
      int col;
      IndexerKelvinRepr3::ToRowCol(flat, row, col);
      ASSERT_EQ(flat, indexer.ToFlat({row,col}));
   }
}




