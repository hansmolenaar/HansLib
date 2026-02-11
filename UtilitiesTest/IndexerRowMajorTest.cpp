#include <gtest/gtest.h>

#include "IndexerRowMajor.h"

TEST(IndexerRowMajorTest, TestToFlat)
{
    IndexerRowMajor<int> ij({2, 3});
    ASSERT_EQ(ij.toFlat({0, 0}), 0);
    ASSERT_EQ(ij.toFlat({0, 1}), 1);
    ASSERT_EQ(ij.toFlat({0, 2}), 2);
    ASSERT_EQ(ij.toFlat({1, 0}), 3);
    ASSERT_EQ(ij.toFlat({1, 1}), 4);
    ASSERT_EQ(ij.toFlat({1, 2}), 5);
}

TEST(IndexerRowMajorTest, TestErrorToFlat)
{
    IndexerRowMajor<int> ij({2, 3});
    // ASSERT_ANY_THROW(ij.toFlat({1}););
    ASSERT_ANY_THROW(ij.toFlat({1, 2, 3}););
    ASSERT_ANY_THROW(ij.toFlat({-1, 0}););
    ASSERT_ANY_THROW(ij.toFlat({2, 0}););
    ASSERT_ANY_THROW(ij.toFlat({0, -1}););
    ASSERT_ANY_THROW(ij.toFlat({0, 3}););
}

TEST(IndexerRowMajorTest, TestDimensions)
{
    IndexerRowMajor<int> ij({2, 3});
    ASSERT_EQ(2, ij.GetRowDimension());
    ASSERT_EQ(3, ij.GetColDimension());
}
