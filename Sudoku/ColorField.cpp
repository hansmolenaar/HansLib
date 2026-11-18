#include "ColorField.h"
#include "MyAssert.h"
#include "SudokuUtils.h"

#include <algorithm>
#include <iterator>
#include <ostream>
#include <stdexcept>

using namespace Sudoku;
using namespace Utilities;
namespace str = std::ranges;

std::string Sudoku::toString(Color color)
{
   if (color == Color::Blue)
   {
      return "BLUE";
   }
   return "GREEN";
}

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

bool ColorField::hasColor(Field field) const
{
   return m_isBlue.test(field) || m_isGreen.test(field);
}

FieldBits ColorField::getFieldsWithColor(Color color) const
{
   if (color == Color::Blue)
   {
      return m_isBlue;
   }
   else
   {
      return m_isGreen;
   }
}

std::ostream &operator<<(std::ostream &os, const Sudoku::ColorField &coloring)
{
   for (Color color : ColorAll)
   {
      os << toString(color) << ":   ";
      const auto fields = coloring.getFieldsWithColor(color);
      for (auto f : SudokuUtils::FieldBitsToRange(fields))
      {
         os << Cell::get(f) << " ";
      }
      os << "\n";
   }
   return os;
}
