#include "CycleNodes.h"
#include "FirstDuplicate.h"
#include "MyException.h"

#include <sstream>

using namespace Topology;
using namespace Utilities;

Topology::CycleNodes::CycleNodes(std::span<const NodeIndex> nodes) :
   m_nodes(nodes.begin(), nodes.end())
{
   if (m_nodes.size() < 3) throw MyException("CycleNodes::CycleNodes too small: size=" + std::to_string(m_nodes.size()));
   const auto duplicates = Utilities::firstDuplicate(m_nodes);
   if (duplicates)
   {
      throw MyException("CycleNodes::CycleNodes has duplicates: " + std::to_string(*duplicates));
   }

   auto minElement = std::min_element(m_nodes.begin(), m_nodes.end());
   std::rotate(m_nodes.begin(), minElement, m_nodes.end());
}

bool CycleNodes::contains(NodeIndex node) const
{
   return str::find(m_nodes, node) != m_nodes.end();
}

std::vector<EdgeNodesDirected> CycleNodes::getEdges() const
{
   std::vector < EdgeNodesDirected> result;
   result.reserve(m_nodes.size());
   for (size_t n = 1; n < m_nodes.size(); ++n)
   {
      result.emplace_back(m_nodes[n - 1], m_nodes[n]);
   }
   result.emplace_back(m_nodes.back(), m_nodes.front());
   return result;
}