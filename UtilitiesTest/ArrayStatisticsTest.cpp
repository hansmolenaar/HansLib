#include <gtest/gtest.h>

#include "ArrayStatistics.h"
#include "Rational.h"

TEST(ArrayStatistics, Empty)
{
   std::vector<double> list{};
   const ArrayStatistics<double> stats(list);
   ASSERT_EQ(stats.getSize(), 0);
}


TEST(ArrayStatistics, List2)
{
   std::vector<int> list{ 4, 3 };
   const ArrayStatistics<int>  stats(list);
   ASSERT_EQ(stats.getSize(), 2);
   ASSERT_EQ(stats.getMinimum(), 3);
   ASSERT_EQ(stats.getMaximum(), 4);
   ASSERT_NEAR(stats.getAverage(), 3.5, 1.0e-12);
}


TEST(ArrayStatistics, ListRational)
{
   std::vector<Rational> list{ Rational(3,1),Rational(4, -1) };
   const ArrayStatistics<Rational>  stats(list);
   ASSERT_EQ(stats.getSize(), 2);
   ASSERT_EQ(stats.getMinimum(), Rational(-4, 1));
   ASSERT_EQ(stats.getMaximum(), Rational(3, 1));
   ASSERT_NEAR(stats.getAverage(), -0.5, 1.0e-12);
}


TEST(ArrayStatistics, ToString)
{
   const std::vector<int> numbers{ 1,2,3,4 };
   const ArrayStatistics<int>  stats(numbers);
   std::ostringstream os;
   os << stats;
   ASSERT_TRUE(os.str().contains("NUM=4  MIN=1  MAX=4  AVG=2.5"));
}


TEST(ArrayStatistics, EmptyToString)
{
   const std::vector<Rational> numbers{};
   const ArrayStatistics<Rational>  stats(numbers);
   std::ostringstream os;
   os << stats;
   ASSERT_EQ(os.str(), "EMPTY");
}