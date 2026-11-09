#pragma once

#include "Cell.h"

namespace Sudoku
{

enum class LinkType
{
   Strong,
   Weak
};

enum class Color
{
   Blue,
   Green
};

// Ordered pair of cells? cell1 < cell2
struct Link
{
   Link(const Cell &, const Cell &, LinkType);
   Cell cell1;
   Cell cell2;
   LinkType type;
};

// A network is connected
class Network
{
 public:
   Network() = default;
   explicit Network(std::span<const Link>);

   std::span<const Cell> getCells() const;

   // Order of cells on input is not relevant
   Link getLink(const Cell &, const Cell &) const;
   std::span<const Cell> getConnectedCells(const Cell &) const;
   Color getColor(const Cell &) const;

 private:
   void setColors(); // On construction?
};

// Collection of links
class Links
{
 public:
   void add(const Link &);
   std::vector<Network> getNetworks() const;
   std::span<Link> get() const;
};

} // namespace Sudoku
