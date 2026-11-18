#include <gtest/gtest.h>

#include "ColorField.h"

using namespace Sudoku;

TEST(ColorFieldTest, Constructor)
{
   static_assert(getOtherColor(Color::Blue) == Color::Green);
   static_assert(getOtherColor(Color::Green) == Color::Blue);
   ColorField colorField;
   ASSERT_TRUE(colorField.empty());
}

TEST(ColorFieldTest, Blue)
{
   const Field field = 42;
   ColorField colorField;
   colorField.set(field, Color::Blue);
   ASSERT_EQ(colorField.get(field), Color::Blue);

   colorField.set(field, Color::Blue);
   ASSERT_EQ(colorField.get(field), Color::Blue);

   ASSERT_ANY_THROW(colorField.set(field, Color::Green));
   ASSERT_EQ(colorField.getFieldsWithColor(Color::Blue).count(), 1);
   ASSERT_EQ(colorField.getFieldsWithColor(Color::Green).count(), 0);
}

TEST(ColorFieldTest, Green)
{
   const Field field = 42;
   ColorField colorField;
   colorField.set(field, Color::Green);
   ASSERT_EQ(colorField.get(field), Color::Green);

   colorField.set(field, Color::Green);
   ASSERT_EQ(colorField.get(field), Color::Green);

   ASSERT_ANY_THROW(colorField.set(field, Color::Blue));

   ASSERT_EQ(colorField.getFieldsWithColor(Color::Blue).count(), 0);
   ASSERT_EQ(colorField.getFieldsWithColor(Color::Green).count(), 1);
}
