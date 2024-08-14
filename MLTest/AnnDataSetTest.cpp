#include <gtest/gtest.h>

#include "AnnDataSet.h" 
#include "MyException.h"

TEST(AnnDataSetTest, Basic)
{
   ML::AnnDataSet dataSet(2, 1);
   ASSERT_EQ(dataSet.getNumberOfSamples(), 0);
   dataSet.addSample({ 1,2 }, { 3 });
   dataSet.addSample({ 4,5 }, { 6 });

   ASSERT_EQ(dataSet.getNumberOfSamples(), 2);

   const auto retvalIn = dataSet.getNthInput(1);
   ASSERT_EQ(retvalIn.size(), 2);
   ASSERT_DOUBLE_EQ(retvalIn[0], 4);
   ASSERT_DOUBLE_EQ(retvalIn[1], 5);

   const auto retvalOut = dataSet.getNthOutput(1);
   ASSERT_EQ(retvalOut.size(), 1);
   ASSERT_DOUBLE_EQ(retvalOut[0], 6);
}


TEST(AnnDataSetTest, Errors)
{
   ML::AnnDataSet dataSet(2, 1);
   ASSERT_THROW(dataSet.getNthInput(0), MyException);
   ASSERT_THROW(dataSet.getNthOutput(0), MyException);
   ASSERT_THROW(dataSet.addSample({ 1 }, { 3 }), MyException);
   ASSERT_THROW(dataSet.addSample({ 1, 2 }, { 3, 4 }), MyException);
}
