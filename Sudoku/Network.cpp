#include "Network.h"
#include "MyAssert.h"

#include <set>

using namespace Sudoku;
using namespace Utilities;

Network::Network(std::span<const Link> links)
{
   for (const auto &link : links)
   {
      add(link);
   }
   setColors();
}

void Network::add(const Link &link)
{
   const auto cellPair = std::make_pair(link.cellMin, link.cellMax);

   const auto found = m_links.find(cellPair);
   if (found != m_links.end())
   {
      // Ignore duplicates, but do not change the type
      MyAssert(found->second == link.type);
   }
   else
   {
      m_links[cellPair] = link.type;
      connect(link.cellMin, link.cellMax);
      connect(link.cellMax, link.cellMin);
   }
}

void Network::connect(const Cell &cellFrom, const Cell &cellTo)
{
   if (m_connections.find(cellFrom) == m_connections.end())
   {
      m_cells.push_back(cellFrom);
   }
   m_connections[cellFrom].push_back(cellTo);
}

std::span<const Cell> Network::getConnectedCells(const Cell &cell) const
{
   return m_connections.at(cell);
}

std::span<const Cell> Network::getCells() const
{
   return m_cells;
}

Link Network::getLink(const Cell &cell1, const Cell &cell2) const
{
   const auto cellPair = std::make_pair(std::min(cell1, cell2), std::max(cell1, cell2));
   const auto found = m_links.find(cellPair);
   MyAssert(found != m_links.end(), "Network::getLink unknown link specified");
   return {cellPair.first, cellPair.second, found->second};
}

void Network::setColors()
{
   if (m_cells.empty())
   {
      return;
   }

   // Initialize
   std::set<Cell> done;
   auto activeCell = m_cells.back();
   std::vector<Cell> todo(1, m_cells.front());
   m_colors[activeCell] = Color::Blue;
   while (!todo.empty())
   {
      activeCell = todo.back();
      todo.pop_back();
      done.insert(activeCell);
      const Color otherColor = getOtherColor(m_colors.at(activeCell));
      for (auto ngb : getConnectedCells(activeCell))
      {
         const auto found = m_colors.find(ngb);
         if (found == m_colors.end())
         {
            m_colors[ngb] = otherColor;
         }
         else
         {
            MyAssert(found->second == otherColor);
         }
         if (!done.contains(ngb))
         {
            todo.push_back(ngb);
         }
      }
   }
}
