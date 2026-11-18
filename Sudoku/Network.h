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

   FieldBits getFields() const;

   // Order of cells on input is not relevant, throws if not found
   Link getLink(Field, Field) const;
   bool contains(Field, Field) const;
   FieldBits getConnectedFields(Field) const;
   static ColorField setColors(const Network &);
   static std::vector<Network> create(std::span<const Link>);

 private:
   // Must be connected!
   explicit Network(std::span<const Link>);

   void add(const Link &);
   void connect(const Cell &, const Cell &);

   FieldBits m_fields;
   std::map<std::pair<Cell, Cell>, LinkType> m_links;
   std::map<Cell, std::vector<Cell>> m_connections;
};

} // namespace Sudoku

std::ostream &operator<<(std::ostream &, const Sudoku::Network &);
