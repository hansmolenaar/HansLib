
#include "TriangleNodesSorted.h"

using namespace Topology;

namespace
{
   std::array<NodeIndex, NumNodesOnTriangle> sort(NodeIndex n0, NodeIndex n1, NodeIndex n2)
   {
      if (n1 < n0) std::swap(n0, n1);
      if (n2 < n0) std::swap(n0, n2);
      if (n2 < n1) std::swap(n1, n2);
      return { n0, n1, n2 };
   }
}

Topology::TriangleNodesSorted::TriangleNodesSorted(NodeIndex n0, NodeIndex n1, NodeIndex n2) :
   m_triangle(sort(n0, n1, n2))
{
}
