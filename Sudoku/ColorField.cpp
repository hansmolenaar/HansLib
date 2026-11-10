#include "ColorField.h"
#include "MyAssert.h"
#include <stdexcept>

using namespace Sudoku;
using namespace Utilities;

bool ColorField::empty() const
{
   return m_isBlue.none() && m_isGreen.none();
}

void ColorField::set(Field field, Color color)
{
   if (color == Color::Blue)
   {
      MyAssert(!m_isGreen.test(field));
      m_isBlue.set(field);
   }
   else
   {
      MyAssert(!m_isBlue.test(field));
      m_isGreen.set(field);
   }
}

void ColorField::set(const Cell &cell, Color color)
{
   set(cell.field, color);
}

Color ColorField::get(Field field) const
{
   if (m_isBlue.test(field))
   {
      return Color::Blue;
   }
   else if (m_isGreen.test(field))
   {
      return Color::Green;
   }
   throw std::runtime_error("No color set for field " + std::to_string(field));
}

Color ColorField::get(const Cell &cell) const
{
   return get(cell.field);
}

bool ColorField::hasColor(Field field) const
{
   return m_isBlue.test(field) || m_isGreen.test(field);
}

bool ColorField::hasColor(const Cell &cell) const
{
   return hasColor(cell.field);
}
