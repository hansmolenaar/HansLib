#include "Network.h"
#include "MyAssert.h"
#include "SudokuDefines.h"

#include <algorithm>
#include <map>
#include <ostream>
#include <set>

using namespace Sudoku;
using namespace Utilities;
namespace str = std::ranges;

std::vector<Network> Network::create(std::span<const Link> links)
{
   if (links.empty())
   {
      return {};
   }

   std::multimap<Cell, Link> cell2link;
   for (const auto link : links)
   {
      cell2link.emplace(link.cellMin, link);
      cell2link.emplace(link.cellMax, link);
   }

   std::vector<Network> result;
   FieldBits done;
   for (Field field = 0; field < NumField; ++field)
   {
      if (done.test(field))
      {
         continue;
      }
      std::vector<Link> component;
      std::vector<Field> todo(1, field);
      while (!todo.empty())
      {
         const Field cur = todo.back();
         todo.pop_back();
         if (!done.test(cur))
         {
            done.set(cur);

            auto range = cell2link.equal_range(Cell::get(cur));
            for (auto itr = range.first; itr != range.second; ++itr)
            {
               component.push_back(itr->second);
               todo.push_back(itr->second.cellMin.field);
               todo.push_back(itr->second.cellMax.field);
            }
         }
      }
      if (!component.empty())
      {
         result.emplace_back(Network(component));
      }
   }
   return result;
}

Network::Network(std::span<const Link> links)
{
   for (const auto &link : links)
   {
      add(link);
   }
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

bool Network::contains(const Cell &cell1, const Cell &cell2) const
{
   const auto cellPair = std::make_pair(std::min(cell1, cell2), std::max(cell1, cell2));
   return m_links.find(cellPair) != m_links.end();
}

ColorField Network::setColors(const Network &netWork)
{
   ColorField result;
   const auto cells = netWork.getCells();
   if (cells.empty())
   {
      return {};
   }

   // Initialize
   std::set<Cell> done;
   auto activeCell = cells.back();
   std::vector<Cell> todo(1, cells.back());
   result.set(activeCell, Color::Blue);
   while (!todo.empty())
   {
      activeCell = todo.back();
      todo.pop_back();
      done.insert(activeCell);
      const Color otherColor = getOtherColor(result.get(activeCell));
      for (auto ngb : netWork.getConnectedCells(activeCell))
      {
         if (!result.hasColor(ngb))
         {
            result.set(ngb, otherColor);
         }
         else if (result.get(ngb) != otherColor)
         {
            // Coloring fails
            return {};
         }
         if (!done.contains(ngb))
         {
            todo.push_back(ngb);
         }
      }
   }

   MyAssert(str::count_if(FieldAll, [&result](Field f) { return result.hasColor(f); }) ==
            static_cast<int>(cells.size()));

   return result;
}

std::ostream &operator<<(std::ostream &os, const Sudoku::Network &network)
{
   std::set<Link> done;
   const auto cells = network.getCells();
   for (auto c : cells)
   {
      const auto ngbs = network.getConnectedCells(c);
      for (const auto ngb : ngbs)
      {
         const auto link = network.getLink(c, ngb);
         if (!done.contains(link))
         {
            done.insert(link);
            os << link << "\n";
         }
      }
   }
   return os;
}
