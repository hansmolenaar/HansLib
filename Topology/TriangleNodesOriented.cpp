
#include "TriangleNodesOriented.h"

using namespace Topology;

namespace
{
   std::array<NodeIndex, NumNodesOnTriangle> order(NodeIndex n0, NodeIndex n1, NodeIndex n2)
   {
      if (n0 < n1 && n0 < n2) return { n0, n1, n2 };
      if (n1 < n0 && n1 < n2) return { n1, n2, n0 };
      if (n2 < n0 && n2 < n1) return { n2, n0, n1 };
      throw MyException("order should not get here");
   }
}

Topology::TriangleNodesOriented::TriangleNodesOriented(NodeIndex n0, NodeIndex n1, NodeIndex n2) :
   m_triangle(order(n0, n1, n2))
{
}
