#pragma once

#include "Cell.h"
#include "SudokuDefines.h"
#include <bitset>

namespace Sudoku
{

enum class Color
{
   Blue,
   Green
};

constexpr Color getOtherColor(Color color)
{
   if (color == Color::Blue)
      return Color::Green;
   return Color::Blue;
}

class ColorField
{
 public:
   void set(Field, Color);
   void set(const Cell &, Color);

   // throws if not set
   Color get(Field) const;
   Color get(const Cell &) const;

   bool empty() const;
   bool hasColor(Field) const;
   bool hasColor(const Cell &) const;

 private:
   std::bitset<NumField> m_isBlue;
   std::bitset<NumField> m_isGreen;
};

} // namespace Sudoku
