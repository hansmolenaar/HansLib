#pragma once

#include "ColorField.h"
#include "Link.h"
#include <map>

namespace Sudoku
{

// A network is by definition connected
class Network
{
 public:
   Network() = default;

   std::span<const Cell> getCells() const;

   // Order of cells on input is not relevant, throws if not found
   Link getLink(const Cell &, const Cell &) const;
   std::span<const Cell> getConnectedCells(const Cell &) const;

   static ColorField setColors(const Network &);
   static std::vector<Network> create(std::span<const Link>);

 private:
   // Must be connected!
   explicit Network(std::span<const Link>);

   void add(const Link &);
   void connect(const Cell &, const Cell &);

   std::vector<Cell> m_cells;
   std::map<std::pair<Cell, Cell>, LinkType> m_links;
   std::map<Cell, std::vector<Cell>> m_connections;
};

} // namespace Sudoku
