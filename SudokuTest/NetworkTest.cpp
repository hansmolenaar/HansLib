#include <gtest/gtest.h>

#include "Network.h"
#include "Single.h"
#include "SudokuUtils.h"

using namespace Sudoku;
using namespace SudokuUtils;
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
   const auto fields = Single(networks).getFields();
   ASSERT_EQ(fields.count(), 2);
   ASSERT_EQ(*(FieldBitsToRange(fields).begin()), cell2.field);
   ASSERT_EQ(*((FieldBitsToRange(fields) | std::views::drop(1)).begin()), cell1.field);
   auto connected = networks.front().getConnectedFields(cell1.field);
   ASSERT_EQ(SingleField(connected), cell2.field);
   connected = networks.front().getConnectedFields(cell2.field);
   ASSERT_EQ(SingleField(connected), cell1.field);
   const auto colors = Network::setColors(networks[0]);
   ASSERT_NE(colors.get(cell1.field), colors.get(cell2.field));
   ASSERT_EQ(networks[0].getLink(cell1.field, cell2.field).type, LinkType::Weak);
   ASSERT_EQ(networks[0].getLink(cell2.field, cell1.field).type, LinkType::Weak);
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
   ASSERT_EQ(Single(networks).getFields().count(), 3);

   auto connected = networks.front().getConnectedFields(cell1.field);
   ASSERT_EQ(SingleField(connected), cell2.field);

   connected = networks.front().getConnectedFields(cell3.field);
   ASSERT_EQ(SingleField(connected), cell2.field);

   connected = networks.front().getConnectedFields(cell2.field);
   ASSERT_TRUE(connected.test(cell1.field));
   ASSERT_TRUE(connected.test(cell3.field));

   const auto colors = Network::setColors(networks[0]);
   ASSERT_NE(colors.get(cell1.field), colors.get(cell2.field));
   ASSERT_NE(colors.get(cell2.field), colors.get(cell3.field));
   ASSERT_EQ(colors.get(cell1.field), colors.get(cell3.field));

   ASSERT_TRUE(networks[0].contains(cell2.field, cell1.field));
   ASSERT_FALSE(networks[0].contains(cell1.field, cell3.field));

   ASSERT_EQ(networks[0].getLink(cell1.field, cell2.field).type, LinkType::Weak);
   ASSERT_EQ(networks[0].getLink(cell2.field, cell3.field).type, LinkType::Strong);
   ASSERT_ANY_THROW(networks.front().getLink(cell1.field, cell3.field));
}

TEST(NetworkTest, Triangle)
{
   const Cell cell1 = Cell::get(42);
   const Cell cell2 = Cell::get(0);
   const Cell cell3 = Cell::get(3);
   const Link link1{cell1, cell2, LinkType::Strong};
   const Link link2{cell2, cell3, LinkType::Strong};
   const Link link3{cell3, cell1, LinkType::Strong};
   const std::array<Link, 3> links{link1, link2, link3};
   const auto networks = Network::create(links);
   const auto network = Single(networks);
   const auto colors = Network::setColors(network);
   ASSERT_TRUE(colors.empty());

   ASSERT_EQ(network.getFields().count(), 3);
   const auto connected = network.getConnectedFields(cell3.field);
   ASSERT_EQ(connected.count(), 2);
   ASSERT_TRUE(connected.test(cell1.field));
   ASSERT_TRUE(connected.test(cell2.field));
}

TEST(NetworkTest, TwoDisjointLinks)
{
   const Cell cell1 = Cell::get(1);
   const Cell cell2 = Cell::get(2);
   const Cell cell3 = Cell::get(3);
   const Cell cell4 = Cell::get(4);
   const Link link1{cell1, cell2, LinkType::Weak};
   const Link link2{cell3, cell4, LinkType::Strong};
   const std::array<Link, 2> links{link1, link2};

   const auto networks = Network::create(links);
   ASSERT_EQ(networks.size(), 2);
   ASSERT_EQ(networks[0].getFields().count(), 2);
   ASSERT_EQ(networks[1].getFields().count(), 2);
}
