
#include "Link.h"
#include "MyAssert.h"

#include <ostream>

using namespace Sudoku;

Link::Link(const Cell &cell1, const Cell &cell2, LinkType linkType)
    : cellMin(std::min(cell1, cell2)), cellMax(std::max(cell1, cell2)), type(linkType)
{
   Utilities::MyAssert(cellMin != cellMax, "Link::Link input cells equal");
}

std::ostream &operator<<(std::ostream &stream, const Sudoku::Link &link)
{
   stream << link.cellMin << " <-=>  " << link.cellMax << "\n";
   return stream;
}
