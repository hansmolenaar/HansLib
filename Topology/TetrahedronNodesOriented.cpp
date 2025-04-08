#include "Defines.h"
#include "MyException.h"
#include "TetrahedronNodesOriented.h"

#include <sstream>

using namespace Topology;

TetrahedronNodesOriented::TetrahedronNodesOriented(NodeIndex n0, NodeIndex n1, NodeIndex n2, NodeIndex n3) :
   TetrahedronNodesOriented(std::array<NodeIndex, NumNodesOnTetrehadron>{ n0, n1, n2, n3 })
{
}

TetrahedronNodesOriented::TetrahedronNodesOriented(std::array<NodeIndex, NumNodesOnTetrehadron> nodes) :
   m_nodes(std::move(nodes))
{
   // Check for duplicates
   if (m_nodes[0] == m_nodes[1] || m_nodes[0] == m_nodes[2] || m_nodes[0] == m_nodes[3] || m_nodes[1] == m_nodes[2] || m_nodes[1] == m_nodes[3] || m_nodes[2] == m_nodes[3])
   {
      std::ostringstream os;
      os << "TetrahedronNodesOriented invalid: " << *this;
      throw MyException(os.str());
   }

   int numSwaps = 0;
   auto mm = str::minmax_element(m_nodes);
   if (mm.min != m_nodes.begin())
   {
      // Keep track of the max-element if we are about to move it
      if (mm.max == m_nodes.begin())
      {
         mm.max = mm.min;
      }
      numSwaps += 1;
      std::swap(*mm.min, m_nodes[0]);
   }
   if (mm.max != m_nodes.begin() + 3)
   {
      numSwaps += 1;
      std::swap(*mm.max, m_nodes[3]);
   }
   // Extra swap to needed to preserve the orientation?
   if (numSwaps == 1)
   {
      std::swap(m_nodes[1], m_nodes[2]);
   }
}

bool TetrahedronNodesOriented::contains(NodeIndex node) const
{
   return node == m_nodes[0] || node == m_nodes[1] || node == m_nodes[2] || node == m_nodes[3];
}

size_t TetrahedronNodesOriented::find(NodeIndex node) const
{
   if (node == m_nodes[0]) return 0;
   if (node == m_nodes[1]) return 1;
   if (node == m_nodes[2]) return 2;
   if (node == m_nodes[3]) return 3;
   return NodeIndexInvalid;
}

std::array<TriangleNodes, NumFacesOnTetrehadron> TetrahedronNodesOriented::getFaces() const
{
   return {
      TriangleNodes{m_nodes[0], m_nodes[1], m_nodes[3]},
      TriangleNodes{m_nodes[0], m_nodes[2], m_nodes[1]},
      TriangleNodes{m_nodes[0], m_nodes[3], m_nodes[2]},
      TriangleNodes{m_nodes[1], m_nodes[2], m_nodes[3]} };
}