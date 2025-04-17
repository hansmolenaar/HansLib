#include <gtest/gtest.h>

#include "FirstDuplicate.h"
#include <vector>

using namespace Utilities;

TEST(FirstDuplicateTest, FirstDuplicate)
{
   std::vector<int> values;
   ASSERT_FALSE(firstDuplicate(values));
   values.push_back(1);
   ASSERT_FALSE(firstDuplicate(values));
   values.push_back(2);
   ASSERT_FALSE(firstDuplicate(values));

   values.push_back(1);
   ASSERT_EQ(*firstDuplicate(values), 1);
}

TEST(FirstDuplicateTest, ThrowOnDuplicate)
{
   throwOnDuplicate(std::vector<int>{});

   throwOnDuplicate(std::vector<int>{42});

   throwOnDuplicate(std::vector<int>{42, 1});
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{42, 42}), "Container has duplicate: 42");

   throwOnDuplicate(std::vector<int>{1, 2, 3});
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{42, 42, 1}), "Container has duplicate: 42");
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{42, 1, 42}), "Container has duplicate: 42");
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{1, 42, 42}), "Container has duplicate: 42");

   throwOnDuplicate(std::vector<int>{1, 2, 3, 4});
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{42, 42, 1, 2}), "Container has duplicate: 42");
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{42, 1, 42, 2}), "Container has duplicate: 42");
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{42, 1, 2, 42}), "Container has duplicate: 42");
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{1, 42, 42, 2}), "Container has duplicate: 42");
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{1, 42, 2, 42}), "Container has duplicate: 42");
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{1, 2, 42, 42}), "Container has duplicate: 42");

   throwOnDuplicate(std::vector<int>{1, 2, 3, 4, 5});
   ASSERT_MYEXCEPTION_MESSAGE(throwOnDuplicate(std::vector<int>{42, 42, 1, 2, 3}), "Container has duplicate: 42");
}