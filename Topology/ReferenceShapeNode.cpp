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
   inline TopologyDimension getMaxTopologyDimension() const override
   {
      return TopologyDimensionDef::Corner;
   }

   inline std::pair<bool, int> getCount(TopologyDimension dim) const override
   {
      Checker.check(dim);
      return std::make_pair(true,  1);
   }

   inline std::pair<bool, const ITopologicalAdjacency*> getAdjacency(TopologyDimension hi, TopologyDimension lo) const override
   {
      Utilities::Assert(hi != lo);
      Checker.check(hi);
      Checker.check(lo);
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