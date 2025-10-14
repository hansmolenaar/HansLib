#include "FirstDuplicate.h"
#include "MyException.h"
#include "PathNodes.h"

#include <sstream>

using namespace Topology;
using namespace Utilities;

Topology::PathNodes::PathNodes(std::span<const NodeIndex> nodes) :
   m_nodes(nodes.begin(), nodes.end())
{
   if (m_nodes.size() < 2) throw MyException("PathNodes::PathNodes too small: size=" + std::to_string(m_nodes.size()));
   Utilities::throwOnDuplicate(m_nodes);

   if (m_nodes.front() > m_nodes.back())
   {
      str::reverse(m_nodes);
   }
}

bool PathNodes::contains(NodeIndex node) const
{
   return str::find(m_nodes, node) != m_nodes.end();
}

std::vector<EdgeNodesDirected> PathNodes::getEdges() const
{
   std::vector < EdgeNodesDirected> result;
   result.reserve(m_nodes.size() - 1);
   for (size_t n = 1; n < m_nodes.size(); ++n)
   {
      result.emplace_back(m_nodes[n - 1], m_nodes[n]);
   }
   return result;
}