#include <gtest/gtest.h>

#include "LocalizationBins.h"

TEST(LocalizationBinsTest, SingleBin)
{
   const Interval<double> intv(2.0, 3.0);
   const LocalizationBins bins = LocalizationBins::CreateUniform(intv, 1);
   ASSERT_EQ(bins.getBinLower(0), intv.getLower());
   ASSERT_EQ(bins.getBinUpper(0), intv.getUpper());

   ASSERT_EQ(bins.find(intv.getLower()), 0);
   ASSERT_EQ(bins.find(2.5), 0);
   ASSERT_EQ(bins.find(intv.getUpper()), 0);
}

TEST(LocalizationBinsTest, TwoBins)
{
   const Interval<double> intv(2.0, 3.0);
   const LocalizationBins bins = LocalizationBins::CreateUniform(intv, 2);
   ASSERT_EQ(bins.getBinLower(0), intv.getLower());
   ASSERT_EQ(bins.getBinUpper(1), intv.getUpper());

   ASSERT_EQ(bins.find(intv.getLower()), 0);
   ASSERT_EQ(bins.find(2.25), 0);
   ASSERT_EQ(bins.find(2.75), 1);
   ASSERT_EQ(bins.find(intv.getUpper()), 1);
}

TEST(LocalizationBinsTest, RangeProblem)
{
   const Interval<double> intv(2.0, 3.0);
   const LocalizationBins bins = LocalizationBins::CreateUniform(intv, 10);
   ASSERT_THROW(bins.find(0.0), MyException);
   ASSERT_THROW(bins.find(4.0), MyException);
}


TEST(LocalizationBinsTest, FromValuesStrictSingleBin)
{
   const std::vector<double> values{ 2.0, 1.0 };
   const LocalizationBins bins = LocalizationBins::CreateFromValues(values, true);
   ASSERT_EQ(bins.getNumBins(), 2);
   ASSERT_EQ(bins.getBinLower(0), 1.0);
   ASSERT_EQ(bins.getBinUpper(1), 2.0);

   ASSERT_EQ(bins.find(1.0), 0);
   ASSERT_EQ(bins.find(1.4), 0);
   ASSERT_EQ(bins.find(1.6), 1);
   ASSERT_EQ(bins.find(2.0), 1);
   ASSERT_THROW(bins.find(0.0), MyException);
   ASSERT_THROW(bins.find(3.0), MyException);
}


TEST(LocalizationBinsTest, FromValuesNonStrictSingleBin)
{
   const LocalizationBins bins = LocalizationBins::CreateFromValues(std::vector<double>{ 2.0, 1.0 }, false);
   ASSERT_EQ(bins.getNumBins(), 4);
   ASSERT_EQ(bins.getBinLower(1), 1.0);
   ASSERT_EQ(bins.getBinUpper(2), 2.0);

   ASSERT_EQ(bins.find(0.0), 0);
   ASSERT_EQ(bins.find(1.4), 1);
   ASSERT_EQ(bins.find(1.6), 2);
   ASSERT_EQ(bins.find(3.0), 3);
}


TEST(LocalizationBinsTest, FromValuesNonStrictSmallBins)
{
   const LocalizationBins bins = LocalizationBins::CreateFromValues(std::vector<double>{ 1.99, 1.01, 2.0, 1.0 }, true, 0.1);
   ASSERT_EQ(bins.getNumBins(), 2);
   ASSERT_EQ(bins.getBinLower(0), 1.0);
   ASSERT_EQ(bins.getBinUpper(1), 2.0);

   ASSERT_EQ(bins.find(1.4), 0);
   ASSERT_EQ(bins.find(1.6), 1);
   ASSERT_EQ(bins.find(1.989), 1);
   ASSERT_EQ(bins.find(1.991), 1);
}


TEST(LocalizationBinsTest, FromValuesUnHappyPath)
{
   const LocalizationBins bins = LocalizationBins::CreateFromValues(std::vector<double> { 2.0, 1.0 }, false);
   ASSERT_THROW(LocalizationBins::CreateFromValues(std::vector<double>{ 1.0 }, false), MyException);
   ASSERT_THROW(LocalizationBins::CreateFromValues(std::vector<double>{ 1.0 }, false, -1.0), MyException);
   ASSERT_THROW(LocalizationBins::CreateFromValues(std::vector<double>{ 1.0, 1.1 }, false, 1.0), MyException);
}


TEST(LocalizationBinsTest, ToString)
{
   const Interval<double> intv(2.0, 3.0);
   const LocalizationBins bins = LocalizationBins::CreateUniform(intv, 2);
   const auto str = bins.toString();
   ASSERT_TRUE(str.contains("LWR=2  UPR=3  NUM=2  MIN=0.5  MAX=0.5  AVG=0.5"));
}