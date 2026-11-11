#include "ColorField.h"
#include "MyAssert.h"

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

std::vector<Cell> ColorField::getCellsWithColor(Color color) const
{
   std::vector<Cell> result;
   str::copy_if(Cell::getAll(), std::back_inserter(result),
                [this, &color](const Cell &cell) { return hasColor(cell) && get(cell) == color; });
   return result;
}

std::ostream &operator<<(std::ostream &os, const Sudoku::ColorField &coloring)
{
   for (Color color : ColorAll)
   {
      os << toString(color) << ":   ";
      const auto cells = coloring.getCellsWithColor(color);
      for (const auto &cell : cells)
      {
         os << cell << " ";
      }
      os << "\n";
   }
   return os;
}
