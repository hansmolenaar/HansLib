#include <gtest/gtest.h>

#include "Utilities/Assert.h"
#include "Utilities/MyException.h"

using namespace Utilities;

TEST(AssertTest, Throws)
{
	ASSERT_NO_THROW(Assert(true));
	ASSERT_THROW(Assert(false), MyException);
}