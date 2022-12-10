#include "Topology/ReferenceShapeNode.h"
#include "Utilities/MyException.h"
#include "Utilities/BoundsCheck.h"
namespace
{
   const auto Checker = BoundsCheck<TopologyDimension>::CreateUpperBound(TopologyDimensionDef::Corner);
}

class NodeAdjacencies : public ITopologicalAdjacencies
{
public:
   TopologyDimension getMaxTopologyDimension() const override
   {
      return TopologyDimensionDef::Corner;
   }

   int getCount(TopologyDimension dim) const override
   {
      Checker.check(dim);
      return 1;
   }

   std::pair<bool, const ITopologicalAdjacency*> getAdjacency(TopologyDimension hi, TopologyDimension lo) const override
   {
      Utilities::Assert(hi != lo);
      return std::make_pair(false, nullptr);
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