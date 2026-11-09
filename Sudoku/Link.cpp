
#include "Link.h"
#include "MyAssert.h"

using namespace Sudoku;

Link::Link(const Cell &cell1, const Cell &cell2, LinkType linkType)
    : cellMin(std::min(cell1, cell2)), cellMax(std::max(cell1, cell2)), type(linkType)
{
   Utilities::MyAssert(cellMin != cellMax, "Link::Link input cells equal");
}
