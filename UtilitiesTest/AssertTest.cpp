#include <gtest/gtest.h>

#include "MyAssert.h"
#include "MyException.h"

using namespace Utilities;

TEST(AssertTest, Throws)
{
    ASSERT_NO_THROW(MyAssert(true));
    ASSERT_MYEXCEPTION_MESSAGE(MyAssert(false), "Assertion failure");
}