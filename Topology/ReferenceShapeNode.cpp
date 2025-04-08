#include "BoundsCheck.h"
#include "MyException.h"
#include "ReferenceShapeNode.h"

using namespace Topology;

namespace
{
   const auto Checker = BoundsCheck<TopologyDimension>::CreateUpperBound(Corner);
}

class NodeAdjacencies : public ITopologicalAdjacencies
{
public:
   inline TopologyDimension getMaxTopologyDimension() const override
   {
      return Corner;
   }

   inline std::optional<int> getCount(TopologyDimension dim) const override
   {
      Checker.check(dim);
      return 1;
   }

   inline std::optional<const ITopologicalAdjacency*> getAdjacency(TopologyDimension hi, TopologyDimension lo) const override
   {
      Utilities::MyAssert(hi != lo);
      Checker.check(hi);
      Checker.check(lo);
      return {};
   }
};

namespace
{
   const NodeAdjacencies Instance;
}

const ITopologicalAdjacencies& ReferenceShapeNode::getAdjacencies() const
{
   return Instance;
}