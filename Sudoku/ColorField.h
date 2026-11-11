#pragma once

#include "Cell.h"
#include "SudokuDefines.h"

namespace Sudoku
{

enum class Color
{
   Blue,
   Green
};
constexpr std::array<Color, 2> ColorAll = {Color::Blue, Color::Green};

constexpr Color getOtherColor(Color color)
{
   if (color == Color::Blue)
      return Color::Green;
   return Color::Blue;
}

std::string toString(Color);

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
   std::vector<Cell> getCellsWithColor(Color) const;

 private:
   FieldBits m_isBlue;
   FieldBits m_isGreen;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::ColorField &);
