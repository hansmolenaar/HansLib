#include <gtest/gtest.h>

#include "Contains.h"
#include "Network.h"
#include "Single.h"

using namespace Sudoku;
using namespace Utilities;

TEST(NetworkTest, EmptyList)
{
   const std::vector<Link> links;
   const auto networks = Network::create(links);
   ASSERT_EQ(networks.size(), 0);
}

TEST(NetworkTest, SingleLink)
{
   const Cell cell1 = Cell::get(42);
   const Cell cell2 = Cell::get(0);
   const Link link{cell1, cell2, LinkType::Weak};
   const auto networks = Network::create(std::span(&link, &link + 1));
   ASSERT_EQ(networks.size(), 1);
   ASSERT_EQ(Single(networks).getCells().size(), 2);
   ASSERT_EQ(Single(networks).getCells().front(), cell2);
   ASSERT_EQ(Single(networks).getCells().back(), cell1);
   auto connected = networks.front().getConnectedCells(cell1);
   ASSERT_EQ(Single(connected), cell2);
   connected = networks.front().getConnectedCells(cell2);
   ASSERT_EQ(Single(connected), cell1);
   const auto colors = Network::setColors(networks[0]);
   ASSERT_NE(colors.get(cell1), colors.get(cell2));
   ASSERT_EQ(networks[0].getLink(cell1, cell2).type, LinkType::Weak);
   ASSERT_EQ(networks[0].getLink(cell2, cell1).type, LinkType::Weak);
}

TEST(NetworkTest, TwoLinks)
{
   const Cell cell1 = Cell::get(42);
   const Cell cell2 = Cell::get(0);
   const Cell cell3 = Cell::get(3);
   const Link link1{cell1, cell2, LinkType::Weak};
   const Link link2{cell2, cell3, LinkType::Strong};
   const std::array<Link, 2> links{link1, link2};

   const auto networks = Network::create(links);
   ASSERT_EQ(networks.size(), 1);
   ASSERT_EQ(Single(networks).getCells().size(), 3);

   auto connected = networks.front().getConnectedCells(cell1);
   ASSERT_EQ(Single(connected), cell2);

   connected = networks.front().getConnectedCells(cell3);
   ASSERT_EQ(Single(connected), cell2);

   connected = networks.front().getConnectedCells(cell2);
   ASSERT_TRUE(Contains(connected, cell1));
   ASSERT_TRUE(Contains(connected, cell3));

   const auto colors = Network::setColors(networks[0]);
   ASSERT_NE(colors.get(cell1), colors.get(cell2));
   ASSERT_NE(colors.get(cell2), colors.get(cell3));
   ASSERT_EQ(colors.get(cell1), colors.get(cell3));

   ASSERT_EQ(networks[0].getLink(cell1, cell2).type, LinkType::Weak);
   ASSERT_EQ(networks[0].getLink(cell2, cell3).type, LinkType::Strong);
   ASSERT_ANY_THROW(networks.front().getLink(cell1, cell3));
}
