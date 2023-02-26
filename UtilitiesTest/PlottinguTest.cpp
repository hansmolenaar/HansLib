#include <gtest/gtest.h>

#include "Plotting.h"
#include <sstream>

TEST(PlottingTest, ToStream)
{
   const std::vector<std::string> headers{ "foo", "bar" };
   const std::vector<std::vector<double>> values{ {1.0, 2.0}, {3.0, 4.0} };
   std::ostringstream sstring;
   Plotting::ToStream(sstring, headers, values);
   const std::string str = sstring.str();
   ASSERT_EQ(str, "foo , bar\n1 , 2\n3 , 4\n");
}

TEST(PlottingTest, PlotFunctions)
{
   const auto fie2 = [](double x) {return x * x; };
   const auto fie3 = [](double x) {return x * x * x; };
   const auto result = Plotting::PlotFunction({ fie2, fie3 }, 1.0, 3.0, 3);
   ASSERT_EQ(result, "x , y0 , y1\n1 , 1 , 1\n2 , 4 , 8\n3 , 9 , 27\n");
}


TEST(PlottingTest, Testje)
{
   const auto fie = [](double x, double y) {return x * x + y; };
   std::vector<std::vector<double>> values;
   values.push_back(std::vector<double>{0.0, 0.0, fie(0, 0)});
   values.push_back(std::vector<double>{1.0, 0.0, fie(1, 0)});
   values.push_back(std::vector<double>{0.0, 1.0, fie(0, 1)});
   values.push_back(std::vector<double>{1.0, 1.0, fie(1, 1)});

   std::ostringstream sstring;
   Plotting::ToStream(sstring, std::vector < std::string >{"x", "y", "z"}, values);
   auto tmp = sstring.str();
}
