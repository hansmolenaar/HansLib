#pragma once

#include "Link.h"
#include <map>

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

// A network is by definition connected
class Network
{
 public:
   Network() = default;

   std::span<const Cell> getCells() const;

   // Order of cells on input is not relevant, throws if not found
   Link getLink(const Cell &, const Cell &) const;
   std::span<const Cell> getConnectedCells(const Cell &) const;
   Color getColor(const Cell &) const;

   static std::vector<Network> create(std::span<const Link>);

 private:
   // Must be connected, throw otherwise
   explicit Network(std::span<const Link>);

   void add(const Link &);
   void connect(const Cell &, const Cell &);
   void setColors();

   std::vector<Cell> m_cells;
   std::map<std::pair<Cell, Cell>, LinkType> m_links;
   std::map<Cell, std::vector<Cell>> m_connections;
   std::map<Cell, Color> m_colors;
};

} // namespace Sudoku
