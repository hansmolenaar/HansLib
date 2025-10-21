#include <gtest/gtest.h>

#include "PointClose.h"
#include "PointReflection.h"

TEST(PointReflectionTest, Basic)
{
    const PointClose<int, 2> areClose;
    const PointReflection<int, 2> reflect;
    ASSERT_TRUE(areClose(reflect(IntPoint2{3, 2}), IntPoint2{-3, -2}));
}