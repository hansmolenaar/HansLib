#include <gtest/gtest.h>

#include "Link.h"
#include "SudokuDefines.h"

using namespace Sudoku;

TEST(LinkTest, Constructor)
{
   const Cell cell1 = Cell::get(42);
   const Cell cell2 = Cell::get(0);
   Link link(cell1, cell2, LinkType::Weak);
   ASSERT_EQ(link.cellMin, cell2);
   ASSERT_EQ(link.cellMax, cell1);
   ASSERT_EQ(link.type, LinkType::Weak);

   link = Link(cell2, cell1, LinkType::Strong);
   ASSERT_EQ(link.cellMin, cell2);
   ASSERT_EQ(link.cellMax, cell1);
   ASSERT_EQ(link.type, LinkType::Strong);
}

TEST(LinkTest, ConstructorInvalid)
{
   const Cell cell1 = Cell::get(42);
   Link link;
   ASSERT_ANY_THROW(link = Link(cell1, cell1, LinkType::Weak));
}
