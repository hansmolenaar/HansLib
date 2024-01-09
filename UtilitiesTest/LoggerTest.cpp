#include <gtest/gtest.h>

#include "Logger.h"

TEST(LoggerTest, HelloWorld)
{
   Logger logger;
   const std::string hello = "Hello world!";
   logger.logLine(hello);
   ASSERT_EQ(logger.get().size(), 1);
   ASSERT_TRUE(logger.get().at(0).contains(hello));
}


TEST(LoggerTest, Lines)
{
   Logger logger;
   logger.logLines(std::vector<std::string>{"Hello","World"});
   ASSERT_EQ(logger.get().size(), 1);
   ASSERT_TRUE(logger.get().at(0).contains("Hello"));
   ASSERT_TRUE(logger.get().at(0).contains("World"));
}